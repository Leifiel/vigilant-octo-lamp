#include <stddef.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include "includes/kek.h"

int get_file(const char *path, struct stat *s)
{
    int fd = 0;

    fd = open(path, O_RDONLY);
    if (fd == -1) {
        return (fd);
    }
    if (fstat(fd, s) == -1) {
        return (-1);
    }
    return (fd);
}

int analyze(const char *path)
{
    int fd = 0;
    void *buf = NULL;
    Elf64_Ehdr *e_header = NULL;
    struct stat s;

    fd = get_file(path, &s);
    if (fd == -1)
        return (2);
    buf = mmap(NULL, (size_t)s.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (!buf) {
        perror("mmap");
        return (-1);
    }
    e_header = buf;
    if (sym_get(buf, *e_header) == 1)
        printf("no sym: %s\n", path);
    close(fd);
    return (0);
}