#include <stdio.h>
#include "includes/kek.h"

int main(int ac, char *av[])
{
    if (ac == 1) {
        analyze("a.out");
        return (SUCCESS);
    }
    if (ac == 2) {
        if (analyze(av[1]) == -1)
            return (ERROR);
        return (SUCCESS);
    }
    for (int i = 1; i < ac; ++i) {
        printf("\n%s:\n", av[i]);
        if (analyze(av[i]) == -1)
            return (ERROR);
    }
    return (SUCCESS);
}