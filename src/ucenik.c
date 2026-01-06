#include <stdio.h>
#include <string.h>
#include "ucenik.h"

void pregledIzostanakaUcenik()
{
    FILE* f = fopen("izostanci.txt", "r");
    char linija[600];
    char ime[50], prezime[50];
    int ima = 0;

    printf("\n=== MOJI IZOSTANCI ===\n");

    printf("Ime: ");
    scanf("%49s", ime);

    printf("Prezime: ");
    scanf("%49s", prezime);

    if (!f) {
        printf("Nema zabiljezenih izostanaka.\n");
        return;
    }

    while (fgets(linija, sizeof(linija), f)) {
        if (strstr(linija, ime) && strstr(linija, prezime)) {
            printf("%s", linija);
            ima = 1;
        }
    }

    fclose(f);

    if (!ima)
        printf("Nema zabiljezenih izostanaka.\n");
}

void ucenikMeni()
{
    int izbor;
    do {
        printf("\n=== UCENIK MENI ===\n");
        printf("1. Pregled mojih izostanaka\n");
        printf("0. Izlaz\n");
        printf("Izbor: ");
        scanf("%d", &izbor);

        switch (izbor) {
            case 1:
                pregledIzostanakaUcenik();
                break;
            case 0:
                printf("Izlaz iz ucenickog menija.\n");
                break;
            default:
                printf("Nepoznata opcija.\n");
        }
    } while (izbor != 0);
}
