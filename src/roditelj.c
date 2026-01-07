#include <stdio.h>
#include <string.h>
#include "roditelj.h"
#include "nastavnik.h" 
#include "ucenik.h"

void roditeljMeni()
{
    int izbor;
    do {
        printf("\n=== RODITELJ MENI ===\n");
        printf("1. Unos opravdanja izostanka\n");
        printf("2. Pregled izostanaka djeteta\n");
        printf("3. Pregled ocjena djeteta\n");
        printf("0. Izlaz\n");
        printf("Izbor: ");
        scanf("%d", &izbor);

        switch (izbor) {
            case 1:
                unesiOpravdanje();
                break;
            case 2:
                pregledIzostanakaRoditelj();
                break;
            case 3:
                pregledOcjenaDjeteta();
                break;
            case 0:
                printf("Izlaz iz roditeljskog menija.\n");
                break;
            default:
                printf("Nepoznata opcija.\n");
        }
    } while (izbor != 0);
}
void pregledIzostanakaRoditelj()
{
    FILE* f = fopen("izostanci.txt", "r");
    char linija[600];
    char ime[50], prezime[50];
    int ima = 0;

    printf("Ime djeteta: ");
    scanf("%49s", ime);

    printf("Prezime djeteta: ");
    scanf("%49s", prezime);

    printf("\n=== IZOSTANCI DJETETA ===\n");

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

void pregledOcjenaDjeteta()
{
    FILE* f = fopen("ocjene.txt", "r");
    char ime[50], prezime[50];
    char predmet[50];
    char linija[200];
    int pronadjeno = 0;

    if (!f) {
        printf("Fajl sa ocjenama ne postoji.\n");
        return;
    }


    printf("\nIme djeteta: ");
    scanf("%49s", ime);

    printf("Prezime djeteta: ");
    scanf("%49s", prezime);

    
    printf("Predmet: ");
    scanf("%49s", predmet);

    printf("\n=== OCJENE DJETETA (%s %s) ===\n", ime, prezime);
    printf("Predmet: %s\n\n", predmet);


    while (fgets(linija, sizeof(linija), f)) {
        if (strstr(linija, ime) &&
            strstr(linija, prezime) &&
            strstr(linija, predmet)) {
            printf("%s", linija);
            pronadjeno = 1;
        }
    }

    fclose(f);

    if (!pronadjeno) {
        printf("Nema zabiljezenih ocjena za izabrani predmet.\n");
    }
}

