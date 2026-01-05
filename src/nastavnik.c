#include <stdio.h>
#include <string.h>
#include "nastavnik.h"

void evidentirajIzostanak()
{
    FILE *f;
    char ime[50], prezime[50];
    char odjeljenje[10];
    char predmet[50];
    char datum[20];

    printf("\n=== EVIDENCIJA IZOSTANKA ===\n");

    printf("Ime ucenika: ");
    scanf("%49s", ime);

    printf("Prezime ucenika: ");
    scanf("%49s", prezime);

    printf("Odjeljenje (npr. 2A): ");
    scanf("%9s", odjeljenje);

    printf("Predmet: ");
    scanf(" %[^\n]%*c", predmet);

    printf("Datum (dd.mm.yyyy): ");
    scanf("%19s", datum);

    f = fopen("izostanci.txt","a");
    if(!f) {
        printf("Greska pri radu sa fajlom.\n");
        return;
    }

    fprintf(f,
        "Ucenik: %s %s | Odjeljenje: %s | Predmet: %s | Datum: %s | Status: neopravdan\n",
        ime, prezime, odjeljenje, predmet, datum);

    fclose(f);

    printf("Izostanak je evidentiran kao NEOPRAVDAN.\n");
}

void nastavnikMeni()
{
    int izbor;
     do {
        printf("\n=== NASTAVNIK MENI ===\n");
        printf("1. Evidencija izostanka ucenika\n");
        printf("0. Izlaz\n");
        printf("Izbor: ");
        scanf("%d", &izbor);

        switch (izbor) {
            case 1:
                evidentirajIzostanak();
                break;
            case 0:
                printf("Izlaz iz nastavnickog menija.\n");
                break;
            default:
                printf("Nepoznata opcija.\n");
        }
    } while (izbor != 0);

}