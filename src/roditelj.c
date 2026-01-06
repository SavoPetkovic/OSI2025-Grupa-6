#include <stdio.h>
#include "roditelj.h"
#include "nastavnik.h" 

void roditeljMeni()
{
    int izbor;
    do {
        printf("\n=== RODITELJ MENI ===\n");
        printf("1. Unos opravdanja izostanka\n");
        printf("0. Izlaz\n");
        printf("Izbor: ");
        scanf("%d", &izbor);

        switch (izbor) {
            case 1:
                unesiOpravdanje();
                break;
            case 0:
                printf("Izlaz iz roditeljskog menija.\n");
                break;
            default:
                printf("Nepoznata opcija.\n");
        }
    } while (izbor != 0);
}
