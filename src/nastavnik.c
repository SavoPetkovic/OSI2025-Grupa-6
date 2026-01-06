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
        printf("2. Unos opravdanja izostanka\n");
        printf("0. Izlaz\n");
        printf("Izbor: ");
        scanf("%d", &izbor);

        switch (izbor) {
            case 1:
                evidentirajIzostanak();
                break;
            case 2:
                unesiOpravdanje();
                break;
            case 0:
                printf("Izlaz iz nastavnickog menija.\n");
                break;
            default:
                printf("Nepoznata opcija.\n");
        }
    } while (izbor != 0);
}


void unesiOpravdanje()
{
    FILE *f = fopen("izostanci.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!f || !temp) {
        printf("Greska pri radu sa fajlovima.\n");
        return;
    }

    char ime[50], prezime[50], datum[20];
    char razlog[100], trajanje[50];
    char linija[600];
    int pronadjen = 0;

    printf("\n=== UNOS OPRAVDANJA IZOSTANKA ===\n");

    printf("Ime ucenika: ");
    scanf("%49s", ime);

    printf("Prezime ucenika: ");
    scanf("%49s", prezime);

    printf("Datum izostanka (dd.mm.yyyy): ");
    scanf("%19s", datum);
    getchar();

    printf("Razlog izostanka: ");
    fgets(razlog, sizeof(razlog), stdin);
    razlog[strcspn(razlog, "\n")] = 0;

    printf("Trajanje (npr. 2 dana): ");
    fgets(trajanje, sizeof(trajanje), stdin);
    trajanje[strcspn(trajanje, "\n")] = 0;

    while (fgets(linija, sizeof(linija), f)) {

        if (strstr(linija, ime) &&
            strstr(linija, prezime) &&
            strstr(linija, datum) &&
            strstr(linija, "Status: neopravdan")) {

            pronadjen = 1;

            fprintf(temp,
                "Ucenik: %s %s | Datum: %s | Status: opravdan | Razlog: %s | Trajanje: %s\n",
                ime, prezime, datum, razlog, trajanje);
        }
        else {
            fputs(linija, temp);
        }
    }

    fclose(f);
    fclose(temp);

    remove("izostanci.txt");
    rename("temp.txt", "izostanci.txt");

    if (pronadjen)
        printf("Opravdanje je uspjesno uneseno.\n");
    else
        printf("Ne postoji neopravdan izostanak za unijete podatke.\n");
}