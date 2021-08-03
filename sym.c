#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "includes/kek.h"

int extract_sections(Elf64_Ehdr header, Elf64_Shdr *sections,
                     char *names, extracted_sec_t **pSecs)
{
    for (int i = 0; i < header.e_shnum; ++i) {
        pSecs[i] = malloc(sizeof(extracted_sec_t));
        if (pSecs[i] == NULL)
            return (-1);
        pSecs[i]->name = strdup(&names[sections[i].sh_name]);
        pSecs[i]->type = sections[i].sh_type;
        pSecs[i]->flags = (int) sections[i].sh_flags;
    }
    return (0);
}

Elf64_Shdr *sym_get_by_name(Elf64_Ehdr header, Elf64_Shdr *headers, char *names,
                    const char *name)
{
    for (int i = 0; i < header.e_shnum; ++i) {
        if (strcmp(&names[headers[i].sh_name], name) == 0)
            return &headers[i];
    }
    return (NULL);
}

int extract_sym(void *buf, Elf64_Shdr *symtab, Elf64_Shdr *strtab,
                extracted_sec_t **pExtracted_sec)
{
    Elf64_Sym *elf64_sym = buf + symtab->sh_offset;
    char *str = buf + strtab->sh_offset;
    int nb_entry = (int) (symtab->sh_size / symtab->sh_entsize);
    extracted_sym_t **pSym = malloc(nb_entry * sizeof(extracted_sym_t*));

    if (pSym == NULL)
        return (-1);
    for (int i = 0; i < nb_entry; ++i) {
        pSym[i] = malloc(sizeof(extracted_sym_t));
        if (pSym[i] == NULL)
            return (-1);
        pSym[i]->sym = malloc(sizeof(Elf64_Sym));
        pSym[i]->name= strdup(&str[elf64_sym[i].st_name]);
        pSym[i]->type= ELF64_ST_TYPE(elf64_sym[i].st_info);
        pSym[i]->bind= ELF64_ST_BIND(elf64_sym[i].st_info);
        memcpy(pSym[i]->sym, &elf64_sym[i], sizeof(Elf64_Sym));
    }
    sort_sym(pSym, nb_entry);
    print_syms(pSym, nb_entry, pExtracted_sec);
    return (0);
}

int sym_get(void *buf, Elf64_Ehdr header)
{
    Elf64_Shdr *headers = buf + header.e_shoff;
    extracted_sec_t **pSections = NULL;
    Elf64_Shdr *symtab = NULL;
    Elf64_Shdr *strtab = NULL;
    char *names = buf + headers[header.e_shstrndx].sh_offset;

    if (header.e_shnum == 0)
        return (1);
    pSections = malloc(header.e_shnum * sizeof(extracted_sec_t));
    if (pSections == NULL)
        return (-1);
    if (extract_sections(header, headers, names, pSections) != 0)
        return (-1);
    symtab = sym_get_by_name(header, headers, names, ".symtab");
    strtab = sym_get_by_name(header, headers, names, ".strtab");
    if (symtab == NULL || strtab == NULL)
        return (1);
    if (extract_sym(buf, symtab, strtab, pSections) == -1)
        return (-1);
    return (0);
}