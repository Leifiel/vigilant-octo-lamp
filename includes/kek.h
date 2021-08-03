#ifndef kekeijf
#define kekeijf

#include <elf.h>

#define SUCCESS     0
#define ERROR       84

typedef struct extracted_sym_s {
    int         type;
    int         bind;
    char        *name;
    Elf64_Sym   *sym;
} extracted_sym_t;

typedef struct extracted_sec_s {
    int     type;
    int     flags;
    char    *name;
} extracted_sec_t;

int sym_get(void *buf, Elf64_Ehdr header);
int analyze(const char *path);
void sort_sym(extracted_sym_t **pExtracted_sym, int nb_entry);
void print_syms(extracted_sym_t **pSym, int nb_entry, extracted_sec_t **pSec);

#endif
