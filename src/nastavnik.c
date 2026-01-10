#include <stdio.h>
#include <string.h>
#include "nastavnik.h"
#include "raspored.h"

void evidentirajIzostanak()
{
    FILE *fUcenici, *fIzostanci;
    char imena[100][50];
    char prezimena[100][50];
    char odjeljenjeInput[10];
    char predmet[50];
    char datum[20];
    char linija[300];
    int brojUcenika = 0;

    printf("\n=== EVIDENCIJA IZOSTANKA ===\n");

    printf("Odjeljenje (npr. 2A): ");
    scanf("%9s", odjeljenjeInput);

    fUcenici = fopen("ucenici_odjeljenja.txt","r");
    if (!fUcenici) {
        printf("Greska: ne mogu otvoriti ucenici_odjeljenja.txt\n");
        return;
    }
    printf("\nUcenici u odjeljenju %s:\n", odjeljenjeInput);
    while (fgets(linija, sizeof(linija), fUcenici)) {
        char ime[50], prezime[50], email[50], odj[10];
        if (sscanf(linija,
               "Ucenik: %49s %49s | Email: %49s | Odjeljenje: %9s",
               ime, prezime, email, odj) == 4)
        {
            
            if (strcmp(odj, odjeljenjeInput) == 0)
            {
                strcpy(imena[brojUcenika], ime);
                strcpy(prezimena[brojUcenika], prezime);
                printf("%d. %s %s\n", brojUcenika + 1, ime, prezime);
                brojUcenika++;
            }
        }
    }
    fclose(fUcenici);

    if(brojUcenika == 0) {
        printf("U odjeljenju nema registrovanih ucenika.\n");
        return;
    }

    int izbor;
    do {
        printf("\nOdaberite ucenika (1-%d): ", brojUcenika);
        scanf("%d", &izbor);
    } while (izbor < 1 || izbor > brojUcenika);

    printf("Predmet: ");
    scanf(" %[^\n]%*c", predmet);

    printf("Datum (dd.mm.yyyy): ");
    scanf("%19s", datum);

    fIzostanci = fopen("izostanci.txt","a");
    if(!fIzostanci) {
        printf("Greska pri otvaranju izostanci.txt.\n");
        return;
    }

    fprintf(fIzostanci,
        "Ucenik: %s %s | Odjeljenje: %s | Predmet: %s | Datum: %s | Status: neopravdan\n",
        imena, prezimena, odjeljenjeInput, predmet, datum);

    fclose(fIzostanci);

    printf("\nIzostanak za učenika %s %s je evidentiran kao NEOPRAVDAN.\n",
           imena[izbor - 1], prezimena[izbor - 1]);}

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
        printf("6. Pregled rasporeda casova\n");
        printf("7. Pregled ocjena odjeljenja (razrednik)\n");
        printf("8. Generisanje izvjestaja o uspjehu ucenika\n");
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
            case 6:
                pregledRasporeda();
                break;
            case 7:
                pregledOcjenaOdjeljenja();
                break;
            case 8: 
                generisanjeIzvjestajUspjehaUcenika(); 
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

        if (sscanf(linija,
            "Ime: %*[^|]| Prezime: %*[^|]| Odjeljenje: %*[^|]| Predmet: %*[^|]| Ocjena: %d",
            &ocjena) == 1)
        {
            printf("- %d\n", ocjena);
            suma += ocjena;
            broj++;
        }
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
void pregledOcjenaOdjeljenja()
{
    FILE *fOdjeljenja, *fUcenici, *fOcjene;
    char linija[600];

    char razrednikIme[50], razrednikPrezime[50];
    char razred[5] = "", oznaka[5] = "";
    int pronadjen = 0;

    printf("\n=== PREGLED OCJENA ODJELJENJA (RAZREDNIK) ===\n");


    printf("Unesite svoje ime: ");
    scanf("%49s", razrednikIme);

    printf("Unesite svoje prezime: ");
    scanf("%49s", razrednikPrezime);

    /* 2. Pronadji odjeljenje gdje je razrednik */
    fOdjeljenja = fopen("odjeljenja.txt", "r");
    if (!fOdjeljenja) {
        printf("Greska: ne mogu otvoriti odjeljenja.txt\n");
        return;
    }

    while (fgets(linija, sizeof(linija), fOdjeljenja)) {
        if (strstr(linija, razrednikIme) && strstr(linija, razrednikPrezime)) {
            sscanf(linija,
                "Razred: %4[^|]| Oznaka: %4[^|]",
                razred, oznaka);
            pronadjen = 1;
            break;
        }
    }
    fclose(fOdjeljenja);

    if (!pronadjen) {
        printf("Vi niste razrednik nijednog odjeljenja.\n");
        return;
    }

    char mojeOdjeljenje[10];
    sprintf(mojeOdjeljenje, "%s%s", razred, oznaka);

    printf("\nVase odjeljenje: %s\n", mojeOdjeljenje);

    
    fUcenici = fopen("ucenici_odjeljenja.txt", "r");
    if (!fUcenici) {
        printf("Nema ucenika u sistemu.\n");
        return;
    }

    char ime[50], prezime[50], email[50], odjeljenje[10];
    int imaUcenika = 0;

    printf("\nUcenici u odjeljenju:\n");
    while (fgets(linija, sizeof(linija), fUcenici)) {
        sscanf(linija,
            "Ucenik: %49s %49s | Email: %49[^|]| Odjeljenje: %9s",
            ime, prezime, email, odjeljenje);

        if (strcmp(odjeljenje, mojeOdjeljenje) == 0) {
            printf("- %s %s\n", ime, prezime);
            imaUcenika = 1;
        }
    }
    fclose(fUcenici);

    if (!imaUcenika) {
        printf("Nema ucenika u ovom odjeljenju.\n");
        return;
    }

    /* 4. Izbor ucenika */
    printf("\nUnesite ime ucenika: ");
    scanf("%49s", ime);

    printf("Unesite prezime ucenika: ");
    scanf("%49s", prezime);

    /* 5. Prikaz predmeta */
    fOcjene = fopen("ocjene.txt", "r");
    if (!fOcjene) {
        printf("Nema evidentiranih ocjena.\n"); 
        return;
    }

    char predmet[50];
    int imaPredmeta = 0;

    printf("\nPredmeti:\n");
    while (fgets(linija, sizeof(linija), fOcjene)) {
        if (strstr(linija, ime) &&
            strstr(linija, prezime) &&
            strstr(linija, mojeOdjeljenje)) {

            sscanf(linija,
                "%*[^P]Predmet: %49[^|]",
                predmet);
            printf("- %s\n", predmet);
            imaPredmeta = 1;
        }
    }
    fclose(fOcjene);

    if (!imaPredmeta) {
        printf("Ucenik nema ocjena.\n");
        return;
    }

    /* 6. Izbor predmeta */
    printf("\nIzaberite predmet: ");
    scanf(" %[^\n]%*c", predmet);

    /* 7. Prikaz ocjena */
    fOcjene = fopen("ocjene.txt", "r");
    printf("\nOcjene iz predmeta %s:\n", predmet);

    int imaOcjena = 0;
    while (fgets(linija, sizeof(linija), fOcjene)) {
        if (strstr(linija, ime) &&
            strstr(linija, prezime) &&
            strstr(linija, mojeOdjeljenje) &&
            strstr(linija, predmet)) {

            printf("%s", linija);
            imaOcjena = 1;
        }
    }
    fclose(fOcjene);

    if (!imaOcjena) {
        printf("Nema ocjena za izabrani predmet.\n");
    }
}

void generisanjeIzvjestajUspjehaUcenika()
{
    char ime[50], prezime[50];
    char linija[600];
    int imaZakljucnih = 0;

    printf("\n=== GENERISANJE IZVJESTAJA O USPJEHU ===\n");

    printf("Ime ucenika: ");
    scanf("%49s", ime);

    printf("Prezime ucenika: ");
    scanf("%49s", prezime);

    FILE *fZak = fopen("zakljucne_ocjene.txt", "r");
    if (!fZak) {
        printf("Greska: zakljucne_ocjene.txt ne postoji.\n");
        return;
    }

     while (fgets(linija, sizeof(linija), fZak)) {
        if (strstr(linija, ime) && strstr(linija, prezime)) {
            imaZakljucnih = 1;
            break;
        }
    }
    fclose(fZak);

    if (!imaZakljucnih) {
        printf("Ucenik nema zakljucnih ocjena. Izvjestaj se ne moze generisati.\n");
        return;
    }

     char nazivFajla[200];
    sprintf(nazivFajla, "izvjestaj_%s_%s.txt", ime, prezime);

    FILE *fOut = fopen(nazivFajla, "w");
    FILE *fIn = fopen("zakljucne_ocjene.txt", "r");

    fprintf(fOut, "=== IZVJESTAJ O USPJEHU ===\n");
    fprintf(fOut, "Ucenik: %s %s\n", ime, prezime);
    fprintf(fOut, "---------------------------------------\n");

    while (fgets(linija, sizeof(linija), fIn)) {
        if (strstr(linija, ime) && strstr(linija, prezime)) {
            char predmet[50];
            int ocjena;
            if (sscanf(linija,
                   "Ucenik: %*s %*s | Predmet: %49[^|]| Zakljucna: %d",
                   predmet, &ocjena) == 2)
        {
        fprintf(fOut, "%s: %d\n", predmet, ocjena);
        }
    }
}

    fclose(fIn);
    fclose(fOut);

    printf("Izvjestaj generisan: %s\n", nazivFajla);
}
