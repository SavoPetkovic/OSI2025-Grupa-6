#include <stdio.h>
#include <string.h>
#include "ucenik.h"
#include "raspored.h"

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
void trim(char *str)
{
    int i;

  
    for (i = strlen(str) - 1; i >= 0 && str[i] == ' '; i--) {
        str[i] = '\0';
    }

    while (*str == ' ') {
        memmove(str, str + 1, strlen(str));
    }
}


void pregledOcjenaUcenik()
{
    FILE *f;
    char linija[600];

    char ime[50], prezime[50], predmet[50];
    char fIme[50], fPrezime[50], fPredmet[50];
    int ocjena;

    printf("\n=== PREGLED MOJIH OCJENA ===\n");

    printf("Ime: ");
    scanf("%49s", ime);

    printf("Prezime: ");
    scanf("%49s", prezime);

    getchar(); // očisti '\n'

    printf("Unesite naziv predmeta: ");
    fgets(predmet, sizeof(predmet), stdin);
    predmet[strcspn(predmet, "\n")] = 0; // ukloni newline

    f = fopen("ocjene.txt", "r");
    if (!f) {
        printf("Greska pri otvaranju fajla ocjene.txt\n");
        return;
    }

    int imaOcjena = 0;

    while (fgets(linija, sizeof(linija), f)) {
    if (sscanf(linija,
        "Ime: %49[^|]| Prezime: %49[^|]| Odjeljenje: %*[^|]| Predmet: %49[^|]| Ocjena: %d",
        fIme, fPrezime, fPredmet, &ocjena) == 4)
    {
        trim(fIme);
        trim(fPrezime);
        trim(fPredmet);

        if (strcmp(fIme, ime) == 0 &&
            strcmp(fPrezime, prezime) == 0 &&
            strcmp(fPredmet, predmet) == 0)
        {
            printf("- %d\n", ocjena);
            imaOcjena = 1;
        }
    }
}


    fclose(f);

    if (!imaOcjena) {
        printf("Nemate ocjena iz predmeta %s.\n", predmet);
    }
}



void ucenikMeni()
{
    int izbor;
    do {
        printf("\n=== UCENIK MENI ===\n");
        printf("1. Pregled mojih izostanaka\n");
        printf("2. Pregled mojih ocjena\n");
        printf("3. Pregled rasporeda casova\n");
        printf("0. Izlaz\n");
        printf("Izbor: ");
        scanf("%d", &izbor);

        switch (izbor) {
            case 1:
                pregledIzostanakaUcenik();
                break;
            case 2:
                pregledOcjenaUcenik();
                break;
            case 3:
                pregledRasporeda();
                break;

            case 0:
                printf("Izlaz iz ucenickog menija.\n");
                break;
            default:
                printf("Nepoznata opcija.\n");
        }
    } while (izbor != 0);
}
