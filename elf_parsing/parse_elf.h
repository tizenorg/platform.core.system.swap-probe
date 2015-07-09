#ifndef _PARSE_ELF_
#define _PARSE_ELF_

#include <elf.h>

#define SIZEOF_VOID_P 4
#if SIZEOF_VOID_P == 8
typedef Elf64_Ehdr Elf_Ehdr;
typedef Elf64_Shdr Elf_Shdr;
typedef Elf64_Sym  Elf_Sym;
typedef Elf64_Addr Elf_Addr;
typedef Elf64_Word Elf_Word;
typedef Elf64_Rel Elf_Rel;
#define ELFCLASS ELFCLASS64
#define ELF_R_SYM(x) ELF64_R_SYM(x)
#define ELF_ST_TYPE(x) ELF64_ST_TYPE(x)
#elif SIZEOF_VOID_P == 4
typedef Elf32_Ehdr Elf_Ehdr;
typedef Elf32_Shdr Elf_Shdr;
typedef Elf32_Sym  Elf_Sym;
typedef Elf32_Addr Elf_Addr;
typedef Elf32_Word Elf_Word;
typedef Elf32_Rel Elf_Rel;
#define ELFCLASS ELFCLASS32
#define ELF_R_SYM(x) ELF32_R_SYM(x)
#define ELF_ST_TYPE(x) ELF32_ST_TYPE(x)
#else
#error "Unknown void* size"
#endif


struct sym_table_entry {
	const char* name;
	const Elf_Sym *entry;
};

#endif /* _PARSE_ELF_ */
