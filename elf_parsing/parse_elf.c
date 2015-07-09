#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <regex.h>
#include <stdbool.h>
#include <malloc.h>

#include "parse_elf.h"


int print_error(const char* msg)
{
	return fprintf(stderr, "Error: %s\n", msg);
}

size_t fsize(int fd)
{
	struct stat buf;
	if (fstat(fd, &buf) != 0) {
		print_error("cannot get file size");
		return 0;
	}
	return buf.st_size;
}

void *mmap_file(const char *filepath, size_t *len)
{
	int fd = open(filepath, O_RDONLY);
	if (fd < 0)
		return NULL;
	*len = fsize(fd);
	void *mem = mmap(NULL, *len, PROT_READ, MAP_PRIVATE, fd, 0);
	close(fd);
	return mem == MAP_FAILED ? NULL : mem;
}

int check_elf(const void *elf)
{
	const Elf_Ehdr *elf_header = elf;
	int ret = 0;

	if ((elf_header->e_ident[EI_MAG0] != ELFMAG0) ||
		(elf_header->e_ident[EI_MAG1] != ELFMAG1) ||
		(elf_header->e_ident[EI_MAG2] != ELFMAG2) ||
		(elf_header->e_ident[EI_MAG3] != ELFMAG3)) {
		print_error("File is not ELF object file");
		ret = -1;
		goto exit;
	}

	if (elf_header->e_ident[EI_CLASS] != ELFCLASS) {
		print_error("Invalid ELF class");
		ret = -2;
		goto exit;
	}

exit:
	return ret;
}

const Elf_Shdr *get_section_by_index(const void *elf, unsigned int index)
{
	const Elf_Ehdr *elf_header = elf;
	const Elf_Shdr *section_table = elf + elf_header->e_shoff;
	if (index > elf_header->e_shnum)
		return NULL;

	return section_table + index;
}

const Elf_Shdr *get_section_by_name(const void *elf, const char* name)
{
	const Elf_Ehdr *elf_header = elf;
	const Elf_Shdr *section_table = elf + elf_header->e_shoff;
	const Elf_Shdr *string_entry = section_table + elf_header->e_shstrndx;
	const char *string_section = elf + string_entry->sh_offset;
	int i;

	for (i = 0; i != elf_header->e_shnum; ++i) {
		const Elf_Shdr *entry = section_table + i;
		if (!strcmp(name, string_section + entry->sh_name)) {
			return entry;
		}
	}
	return NULL;
}

const Elf_Shdr *get_section_by_type(const void *elf, Elf_Word sh_type)
{
	const Elf_Ehdr *elf_header = elf;
	const Elf_Shdr *section_table = elf + elf_header->e_shoff;
	int i;

	for (i = 0; i != elf_header->e_shnum; ++i) {
		const Elf_Shdr *entry = section_table + i;
		if (entry->sh_type == sh_type)
			return entry;
	}
	return NULL;
}

struct sym_table_entry *get_sym_addr(const void *elf, const char* table_name, const char* sym_names[], size_t *n, bool is_only_func)
{
	const Elf_Shdr *shdr;
	const Elf_Shdr *str_section;
	const Elf_Sym *table;
	const char *name;
	int entries_num;
	int i, j;
	struct sym_table_entry *res;
	regex_t regex;

	shdr = get_section_by_name(elf, table_name);
	if (!shdr || shdr->sh_type == SHT_NOBITS) {
		res = NULL;
		goto exit;
	}

	entries_num = shdr->sh_size / sizeof(Elf_Sym);
	if (!entries_num) {
		res = NULL;
		goto exit;
	}

	str_section = get_section_by_index(elf, ((Elf_Ehdr *)elf)->e_shstrndx);
	if (!str_section) {
		res = NULL;
		goto exit;
	}

	table = elf + shdr->sh_offset;

	str_section = get_section_by_index(elf, shdr->sh_link);
	if (!str_section) {
		res = NULL;
		goto exit;
	}

	if (sym_names)
		res = (struct sym_table_entry *)calloc(*n, sizeof(struct sym_table_entry));
	else {
		*n = entries_num;
		res = (struct sym_table_entry *)calloc(entries_num, sizeof(struct sym_table_entry));
	}

	if (!res)
		goto exit;

	if (sym_names) {
		for (j = 0; j < *n; j++) {
			if (regcomp(&regex, sym_names[j], REG_EXTENDED))
				continue;

			for (i = 0; i < entries_num; i++) {
				name = elf + str_section->sh_offset + table[i].st_name;

				if (is_only_func && ELF_ST_TYPE(table[i].st_info) != STT_FUNC)
					continue;

				if (!res[j].entry && !regexec(&regex, name, 0, NULL, 0)) {
					res[j].name = name;
					res[j].entry = &table[i];
				}
			}

			regfree(&regex);
		}
	} else {
		for (i = 0; i < entries_num; i++) {
			name = elf + str_section->sh_offset + table[i].st_name;
			res[i].name = name;
			res[i].entry = &table[i];
		}
	}

exit:
	return res;
}

int get_got_plt_addrs(const void *elf, const char* entry_names[], Elf_Addr addrs[], size_t n)
{
	const Elf_Shdr *shdr;
	const Elf_Sym *sym_table;
	const char* name;
	int plt_table_size;
	int i, j;
	int ret = 0;
	regex_t regex;

	shdr = get_section_by_name(elf, ".rel.plt");
	if (!shdr || shdr->sh_type == SHT_NOBITS) {
		ret = -1;
		goto exit;
	}

	plt_table_size = shdr->sh_size / sizeof(Elf_Rel);
	const Elf_Rel *plt_table = elf + shdr->sh_offset;

	const Elf_Shdr* tmp_shdr = get_section_by_index(elf, shdr->sh_link);
	sym_table = elf + tmp_shdr->sh_offset;
	tmp_shdr = get_section_by_index(elf, tmp_shdr->sh_link);
	if (!tmp_shdr) {
		ret = -1;
		goto exit;
	}

	for (j = 0; j < n; j++) {
		if (regcomp(&regex, entry_names[j], REG_EXTENDED))
				continue;

		for (i = 0; i < plt_table_size; i++) {
			name = elf + tmp_shdr->sh_offset + sym_table[ELF_R_SYM(plt_table[i].r_info)].st_name;

			if (!regexec(&regex, name, 0, NULL, 0)) {
				addrs[j] = plt_table[i].r_offset;
			}
		}

		regfree(&regex);
	}

exit:
	return ret;
}

int get_plt_addrs(const void *elf, const char* func_names[], Elf_Addr addrs[], size_t n)
{
	int ret = get_got_plt_addrs(elf, func_names, addrs, n);
	if (ret)
		goto exit;

	const Elf_Shdr *got_plt_shdr = get_section_by_name(elf, ".got.plt");
	if (!got_plt_shdr || got_plt_shdr->sh_type == SHT_NOBITS) {
		ret = -1;
		goto exit;
	}

	int base = got_plt_shdr->sh_addr - got_plt_shdr->sh_offset;

	int j;
	for (j = 0; j < n; j++) {
		if (addrs[j])
			addrs[j] = *(Elf_Addr *)(elf + addrs[j] - base) - 6;
	}

	ret = 0;

exit:
	return ret;
}

const char *get_interp(const void *elf)
{
	const Elf_Shdr *shdr = get_section_by_name(elf, ".interp");
	if (shdr && shdr->sh_type != SHT_NOBITS)
		return (const char *)(elf + shdr->sh_offset);
	else return NULL;
}

void add_res_sym(struct sym_table_entry *dst, struct sym_table_entry *src, size_t len)
{
	int i;

	if (!dst || !src)
		return;

	for (i = 0; i < len; i++) {
		if (!dst[i].entry && src[i].entry)
			dst[i] = src[i];
	}
}

void print_sym_table(struct sym_table_entry *entry, size_t len)
{
	int i;

	if (!entry)
		return;

	for (i = 0; i < len; i++) {
		if (entry[i].name && *entry[i].name)
			printf("%08x %s\n", entry[i].entry->st_value, entry[i].name);
	}
}

void print_sym_addrs(struct sym_table_entry *entry, size_t len)
{
	int i;
	int addr;

	if (!entry)
		return;

	for (i = 0; i < len; i++) {
		if (!entry[i].entry)
			addr = 0;
		else
			addr = entry[i].entry->st_value;

		printf("%08x\n", addr);
	}
}

void print_usage(const char* pr_path)
{
	fprintf(stderr,
		"usage: %s path_to_elf %s\n",
		pr_path,
		"[ -i | -sa | -s sym_name1 [sym_name2 ...] | -r rel_name1 [rel_name2 ...]]"
	);
}

int main(int argc, char **argv)
{
	const char *filename = argv[1];
	size_t elf_len;
	void *elf;
	struct sym_table_entry *res, *tmp;
	Elf_Addr *addrs;
	size_t entries_num = 0;
	int i;
	int ret = 0;

	if (argc >= 3) {
		elf = mmap_file(filename, &elf_len);

		if (!elf) {
			print_error("mmap file error");
			ret = -1;
			goto fail_exit;
		}

		if (check_elf(elf)) {
			ret = -1;
			goto fail_exit;
		}

		if (argc == 3) {
			if (!strcmp(argv[2], "-i")) {
				const char* interp = get_interp(elf);
				if (interp)
					printf("%s\n", interp);
			} else if (!strcmp(argv[2], "-sa")) {
				res = get_sym_addr(elf, ".symtab", NULL, &entries_num, false);
				print_sym_table(res, entries_num);
				free(res);

				res = get_sym_addr(elf, ".dynsym", NULL, &entries_num, false);
				print_sym_table(res, entries_num);
				free(res);
			} else {
				ret = -1;
				goto print_usage_exit;
			}

		} else if (argc > 3) {
			const char **names = (const char **)&argv[3];
			entries_num = argc - 3;

			if (!strcmp(argv[2], "-s") || !strcmp(argv[2], "-sf")) {
				bool is_only_func = argv[2][2] == 'f';

				res = get_sym_addr(elf, ".symtab", names, &entries_num, is_only_func);
				tmp = get_sym_addr(elf, ".dynsym", names, &entries_num, is_only_func);

				if (res) {
					add_res_sym(res, tmp, entries_num);
					print_sym_addrs(res, entries_num);
				} else
					print_sym_addrs(tmp, entries_num);

				free(res);
				free(tmp);
			} else if (!strcmp(argv[2], "-r")) {
				addrs = (Elf_Addr *)calloc(entries_num, sizeof(Elf_Addr));
				if (addrs) {
					if (!get_plt_addrs(elf, names, addrs, entries_num)) {
						for (i = 0; i < entries_num; i++)
							printf("%08x\n", addrs[i]);
					} else
						ret = -1;
					free(addrs);
				} else
					ret = -1;
			} else {
				ret = -1;
				goto print_usage_exit;
			}
		} else {
			ret = -1;
			goto print_usage_exit;
		}
	} else {
		ret = -1;
		goto print_usage_exit;
	}

	return ret;

print_usage_exit:
	print_usage(argv[0]);

fail_exit:
	return ret;
}
