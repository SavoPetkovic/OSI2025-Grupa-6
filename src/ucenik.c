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

void pregledOcjenaUcenik() {
    char ime[50], prezime[50];
    char linija[300];
    char predmeti[50][50]; // lista predmeta za učenika
    int predmetCount = 0;
    int i;

    printf("\n=== PREGLED MOJIH OCJENA ===\n");

    printf("Ime: ");
    scanf("%49s", ime);
    printf("Prezime: ");
    scanf("%49s", prezime);

    FILE *f = fopen("ocjene.txt", "r");
    if (!f) {
        printf("Nema evidentiranih ocjena.\n");
        return;
    }

    while (fgets(linija, sizeof(linija), f)) {
        if (strstr(linija, ime) && strstr(linija, prezime)) {
    
            char tempPredmet[50];
            sscanf(linija, "Ime: %*[^|]| Prezime: %*[^|]| Predmet: %49[^|]| Ocjena: %*s", tempPredmet);

            
            int exists = 0;
            for (i = 0; i < predmetCount; i++) {
                if (strcmp(predmeti[i], tempPredmet) == 0) {
                    exists = 1;
                    break;
                }
            }
            if (!exists) {
                strcpy(predmeti[predmetCount], tempPredmet);
                predmetCount++;
            }
        }
    }

    fclose(f);

    if (predmetCount == 0) {
        printf("Nemate zabilježenih ocjena.\n");
        return;
    }

    
    printf("\nPredmeti koje pohađate:\n");
    for (i = 0; i < predmetCount; i++) {
        printf("%d. %s\n", i + 1, predmeti[i]);
    }


    int izbor;
    do {
        printf("Unesite broj predmeta za prikaz ocjena: ");
        scanf("%d", &izbor);
        if (izbor < 1 || izbor > predmetCount) {
            printf("Nepostojeca opcija, pokusajte ponovo.\n");
        }
    } while (izbor < 1 || izbor > predmetCount);

    char izabraniPredmet[50];
    strcpy(izabraniPredmet, predmeti[izbor - 1]);


    printf("\nOcjene za predmet %s:\n", izabraniPredmet);
    f = fopen("ocjene.txt", "r");
    if (!f) {
        printf("Greska pri otvaranju fajla ocjene.txt\n");
        return;
    }

    int imaOcjena = 0;
    while (fgets(linija, sizeof(linija), f)) {
        if (strstr(linija, ime) && strstr(linija, prezime) && strstr(linija, izabraniPredmet)) {
            // Ispis linije
            char predmet[50], ocjena[10];
            sscanf(linija, "Ime: %*[^|]| Prezime: %*[^|]| Predmet: %49[^|]| Ocjena: %9s", predmet, ocjena);
            printf("- %s\n", ocjena);
            imaOcjena = 1;
        }
    }

    fclose(f);

    if (!imaOcjena) {
        printf("Nemate ocjena za ovaj predmet.\n");
    }
}

void ucenikMeni()
{
    int izbor;
    do {
        printf("\n=== UCENIK MENI ===\n");
        printf("1. Pregled mojih izostanaka\n");
        printf("2. Pregled mojih ocjena\n");
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
            case 0:
                printf("Izlaz iz ucenickog menija.\n");
                break;
            default:
                printf("Nepoznata opcija.\n");
        }
    } while (izbor != 0);
}
