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
        printf("3. Unos ocjena\n");
        printf("4. Zakljucivanje ocjena\n");
        printf("5. Pregled izostanaka\n");
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
           case 3:
                unosOcjene();
                break;

           case 4:
                zakljuciOcjenu();
                break;

           case 5:
                pregledIzostanakaNastavnik();
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

void zakljuciOcjenu()
{
    FILE *fOcjene, *fZakljucne, *fPeriod;
    char ime[50], prezime[50], predmet[50];
    char linija[300];
    int ocjena, suma = 0, broj = 0;
    float prosjek;
    int zakljucna;
    char potvrda;

    printf("\n=== ZAKLJUCIVANJE OCJENE ===\n");

    /* Provjera perioda */
    fPeriod = fopen("period.txt", "r");
    if (!fPeriod) {
        printf("Period zakljucivanja nije definisan.\n");
        return;
    }

    fgets(linija, sizeof(linija), fPeriod);
    fclose(fPeriod);

    if (strstr(linija, "AKTIVAN") == NULL) {
        printf("Zakljucivanje ocjena trenutno NIJE dozvoljeno.\n");
        return;
    }

    printf("Ime ucenika: ");
    scanf("%49s", ime);

    printf("Prezime ucenika: ");
    scanf("%49s", prezime);

    printf("Predmet: ");
    scanf(" %[^\n]%*c", predmet);

    /* Provjera da li je vec zakljucena ocjena */
    fZakljucne = fopen("zakljucne_ocjene.txt", "r");
    if (fZakljucne) {
        while (fgets(linija, sizeof(linija), fZakljucne)) {
            if (strstr(linija, ime) &&
                strstr(linija, prezime) &&
                strstr(linija, predmet)) {

                printf("Ocjena je vec zakljucena i ne moze se mijenjati.\n");
                fclose(fZakljucne);
                return;
            }
        }
        fclose(fZakljucne);
    }

    /* Citanje ocjena */
    fOcjene = fopen("ocjene.txt", "r");
    if (!fOcjene) {
        printf("Nema unesenih ocjena.\n");
        return;
    }

    printf("\nOcjene:\n");
    while (fgets(linija, sizeof(linija), fOcjene)) {
        if (strstr(linija, ime) &&
            strstr(linija, prezime) &&
            strstr(linija, predmet)) {

            sscanf(linija, "%*[^0-9]%d", &ocjena);
            printf("- %d\n", ocjena);
            suma += ocjena;
            broj++;
        }
    }
    fclose(fOcjene);

    if (broj == 0) {
        printf("Nema dovoljno ocjena za zakljucivanje.\n");
        return;
    }

    prosjek = (float)suma / broj;
    zakljucna = (int)(prosjek + 0.5); // zaokruzivanje

    printf("\nPredlozena zakljucna ocjena: %d\n", zakljucna);
    printf("Zelite li izmijeniti ocjenu? (Y/N): ");
    scanf(" %c", &potvrda);

    if (potvrda == 'Y' || potvrda == 'y') {
        printf("Unesite zakljucnu ocjenu (1-5): ");
        scanf("%d", &zakljucna);
    }

    printf("Potvrditi zakljucivanje? (Y/N): ");
    scanf(" %c", &potvrda);

    if (potvrda != 'Y' && potvrda != 'y') {
        printf("Zakljucivanje otkazano.\n");
        return;
    }

    /* Upis zakljucne ocjene */
    fZakljucne = fopen("zakljucne_ocjene.txt", "a");
    if (!fZakljucne) {
        printf("Greska pri upisu zakljucne ocjene.\n");
        return;
    }

    fprintf(fZakljucne,
        "Ucenik: %s %s | Predmet: %s | Zakljucna: %d\n",
        ime, prezime, predmet, zakljucna);

    fclose(fZakljucne);

    printf("Zakljucna ocjena je USPJESNO sacuvana.\n");
}
void pregledIzostanakaNastavnik()
{
    FILE* f = fopen("izostanci.txt", "r");
    char linija[600];
    int ima = 0;

    printf("\n=== PREGLED IZOSTANAKA ===\n");

    if (!f) {
        printf("Nema zabiljezenih izostanaka.\n");
        return;
    }

    while (fgets(linija, sizeof(linija), f)) {
        printf("%s", linija);
        ima = 1;
    }

    fclose(f);

    if (!ima)
        printf("Nema zabiljezenih izostanaka.\n");
}

void unosOcjene()
{
    FILE *f;
    char ime[50], prezime[50];
    char odjeljenje[10];
    char predmet[50];
    char datum[20];
    int ocjena;
    char izbor;

    printf("\n=== UNOS OCJENE ===\n");

    printf("Ime ucenika: ");
    scanf("%49s", ime);

    printf("Prezime ucenika: ");
    scanf("%49s", prezime);

    printf("Odjeljenje (npr. 2A): ");
    scanf("%9s", odjeljenje);

    printf("Predmet: ");
    scanf(" %[^\n]%*c", predmet);

    do {
        printf("Unesite ocjenu (1-5): ");
        scanf("%d", &ocjena);

        if (ocjena < 1 || ocjena > 5)
            printf("Greska: dozvoljene ocjene su 1–5!\n");

    } while (ocjena < 1 || ocjena > 5);

    printf("Datum (dd.mm.yyyy): ");
    scanf("%19s", datum);

    printf("Potvrdite unos? (Y/N): ");
    scanf(" %c", &izbor);

    if (izbor != 'Y' && izbor != 'y') {
        printf("Unos ocjene otkazan.\n");
        return;
    }

    f = fopen("ocjene.txt", "a");
    if (!f) {
        printf("Greska pri upisu ocjene.\n");
        return;
    }

    fprintf(f,
        "Ime: %s | Prezime: %s | Odjeljenje: %s | Predmet: %s | Ocjena: %d | Datum: %s\n",
        ime, prezime, odjeljenje, predmet, ocjena, datum);

    fclose(f);

    printf("Ocjena uspjesno sacuvana.\n");
}
