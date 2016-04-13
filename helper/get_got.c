#include <stdio.h>
#include <string.h>
#include <link.h>
#include <elf.h>
#include "get_got.h"

#ifdef __mips__
# define D_PTR(map, i) ((map)->i->d_un.d_ptr + (map)->l_addr)
#else /* __mips__ */
# define D_PTR(map, i) (map)->i->d_un.d_ptr
#endif /* __mips__ */


#if (defined __x86_64__) || (defined __aarch64__)
# define PLTREL  ElfW(Rela)
#elif (defined __i386__) || (defined __arm__) || (defined __thumb__)
# define PLTREL  ElfW(Rel)
#else
#error "This arch is not supported"
#endif


#define ELFW(type)	_ElfW (ELF, __ELF_NATIVE_CLASS, type)
#define _ElfW(e,w,t)	_ElfW_1 (e, w, _##t)
#define _ElfW_1(e,w,t)	e##w##t


typedef long int Lmid_t;


struct ext_map {
	struct link_map map;
	struct link_map *l_real;
	Lmid_t l_ns;
	struct libname_list *l_libname;
	ElfW(Dyn) *l_info[0];
};


const char *lmap_get_name(const struct link_map *map, unsigned long offset)
{
	const struct ext_map *emap = (const struct ext_map *)map;
	const char *strtab;
	const PLTREL *reloc;
	const ElfW(Sym) *symtab;
	const ElfW(Sym) *sym;

	symtab = (const void *)D_PTR(emap, l_info[DT_SYMTAB]);
	strtab = (const void *)D_PTR(emap, l_info[DT_STRTAB]);
	reloc = (const void *)(D_PTR(emap, l_info[DT_JMPREL]) + offset);

	sym = &symtab[ELFW(R_SYM)(reloc->r_info)];
	return strtab + sym->st_name;
}

unsigned long lmap_reladdr_by_offset(const struct link_map *map,
				     unsigned long offset)
{
	const struct ext_map *emap = (const struct ext_map *)map;

	return (unsigned long)(D_PTR(emap, l_info[DT_JMPREL]) + offset);
}

unsigned long lmap_reladdr_by_name(const struct link_map *map, const char *name)
{
	const struct ext_map *emap = (const struct ext_map *)map;
	const char *strtab;
	const ElfW(Sym) *symtab;
	ElfW(Word) offset, size;

	/* skip null-name */
	if (name[0] == 0)
		return 0;

	/* relocation table doesn't exist */
	if (emap->l_info[DT_PLTRELSZ] == NULL)
		return 0;

	symtab = (const void *)D_PTR(emap, l_info[DT_SYMTAB]);
	strtab = (const void *)D_PTR(emap, l_info[DT_STRTAB]);
	size = emap->l_info[DT_PLTRELSZ]->d_un.d_val;

	for (offset = 0; offset < size; offset += sizeof(PLTREL)) {
		const PLTREL *reloc;
		const ElfW(Sym) *sym;
		const char *sym_name;

		reloc = (const void *)(D_PTR(emap, l_info[DT_JMPREL]) + offset);
		sym = &symtab[ELFW(R_SYM)(reloc->r_info)];
		sym_name = strtab + sym->st_name;

		if (strcmp(name, sym_name) == 0)
			return map->l_addr + reloc->r_offset;
	}

	return 0;
}
