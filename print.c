#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <locale.h>
#include "includes/kek.h"

void get_type(char *type, extracted_sym_t *sym, extracted_sec_t **pSec)
{
    if (pSec[sym->sym->st_shndx]->type == SHT_NOBITS
        && pSec[sym->sym->st_shndx]->flags == (SHF_ALLOC | SHF_WRITE))
        *type = 'B';
    else if (pSec[sym->sym->st_shndx]->type == SHT_PROGBITS
             && pSec[sym->sym->st_shndx]->flags == SHF_ALLOC)
        *type = 'R';
    else if (pSec[sym->sym->st_shndx]->type == SHT_PROGBITS
             && pSec[sym->sym->st_shndx]->flags == (SHF_ALLOC | SHF_WRITE))
        *type = 'D';
    else if (pSec[sym->sym->st_shndx]->type == SHT_PROGBITS
             && pSec[sym->sym->st_shndx]->flags == (SHF_ALLOC | SHF_EXECINSTR))
        *type = 'T';
    else if (pSec[sym->sym->st_shndx]->type == SHT_DYNAMIC)
        *type = 'D';
}

void get_bind(char *t, int bind, int type, extracted_sym_t *sym)
{
    switch (bind) {
        case STB_GLOBAL:
            *t = (char) toupper(*t);
            break;
        case STB_LOCAL:
            *t = (char) tolower(*t);
            break;
        case STB_WEAK:
            *t = 'W';
            if (sym->sym->st_shndx == SHN_UNDEF)
                *t = 'w';
            if (type == STT_OBJECT) {
                *t = 'V';
                if (sym->sym->st_shndx == SHN_UNDEF)
                    *t = 'v';
            }
            break;
        default:break;
    }
}

void print_sym(int type, int bind, char *name,
               extracted_sym_t *sym, extracted_sec_t **pSec)
{
    char t = 't';

    if (strlen(name) == 0)
        return;
    if (bind == STB_GNU_UNIQUE)
        t = 'u';
    else if (sym->sym->st_shndx == SHN_UNDEF)
        t = 'U';
    else if (sym->sym->st_shndx == SHN_ABS)
        t = 'A';
    else if (sym->sym->st_shndx == SHN_COMMON)
        t = 'C';
    else
        get_type(&t, sym, pSec);
    get_bind(&t, bind, type, sym);
    if (type == STT_FILE)
        return;
    if (sym->sym->st_shndx == SHN_UNDEF)
        printf("                 %c %s\n", t, name);
    else
        printf("%016x %c %s\n", (unsigned int) sym->sym->st_value, t, name);
}

void print_syms(extracted_sym_t **pSym, int nb_entry, extracted_sec_t **pSec)
{
    for (int i = 0; i < nb_entry; ++i) {
        print_sym(pSym[i]->type, pSym[i]->bind, pSym[i]->name, pSym[i], pSec);
    }
}

void sort_sym(extracted_sym_t **pSym, int nb_entry)
{
    extracted_sym_t *temp = NULL;

    setlocale(LC_ALL, "");
    int i = 0, j;
    for(; i < nb_entry; i++) {
        for(j = 0; j < i; j++) {
            if(strcoll(pSym[i]->name, pSym[j]->name) < 0) {
                temp = pSym[i];
                pSym[i] = pSym[j];
                pSym[j] = temp;
            }
        }
    }
}