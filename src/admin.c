#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "admin.h"

void blokirajKorisnika() 
{
    FILE *f = fopen("korisnici.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!f || !temp) {
        printf("Greska pri radu sa fajlovima.\n");
        return;
    }

    char email[50];
    char razlog[100];
    char linija[600];
    int pronadjen = 0;

    printf("Unesite email korisnika za blokiranje: ");
    scanf("%49s", email);
    getchar(); // cisti buffer

    printf("Unesite razlog blokiranja: ");
    fgets(razlog, sizeof(razlog), stdin);
    razlog[strcspn(razlog, "\n")] = 0; // uklanja \n sa fgets

    while (fgets(linija, sizeof(linija), f)) {
        if (strstr(linija, email)) {
            pronadjen = 1;

            if (strstr(linija, "Status: blokiran")) {
                printf("Nalog je vec blokiran.\n");
                fputs(linija, temp);
            } else {
                // Parsiramo sve postojeće podatke
                char ime[50], prezime[50], tip[20], mail[50], lozinka[50];
                char broj[20], adresa[100], datum[20];

                sscanf(linija,
                    "Ime: %49[^|]| Prezime: %49[^|]| Tip: %19[^|]| Email: %49[^|]| Lozinka: %49[^|]| Broj telefona: %19[^|]| Adresa: %99[^|]| Datum rodjenja: %19[^\n]",
                    ime, prezime, tip, mail, lozinka, broj, adresa, datum);

                // Sastavljamo novu liniju sa statusom i razlogom
                char novaLinija[800];
                sprintf(novaLinija,
                    "Ime: %s | Prezime: %s | Tip: %s | Email: %s | Lozinka: %s | Broj telefona: %s | Adresa: %s | Datum rodjenja: %s | Status: blokiran | Razlog: %s\n",
                    ime, prezime, tip, mail, lozinka, broj, adresa, datum, razlog);

                fputs(novaLinija, temp);
                printf("Korisnik uspjesno blokiran.\n");
            }
        } else {
            fputs(linija, temp);
        }
    }

    fclose(f);
    fclose(temp);

    remove("korisnici.txt");
    rename("temp.txt", "korisnici.txt");

    if (!pronadjen) {
        printf("Korisnik nije pronadjen.\n");
    }
}


void adminMeni() 
{
    int izbor;

    do {
        printf("\n=== ADMIN MENI ===\n");
        printf("1. Registracija korisnika\n");
        printf("2. Blokiranje korisnika\n");
        printf("3. Brisanje korisnickog naloga\n");   // 👈 NOVA OPCIJA
        printf("0. Izlaz\n");
        printf("Izbor: ");
        scanf("%d", &izbor);

        switch (izbor) {
            case 1:
                registracijaKorisnika();
                break;
            case 2:
                blokirajKorisnika();
                break;
            case 3:
                obrisiKorisnika();   // 👈 POZIV NOVE FUNKCIJE
                break;
            case 0:
                printf("Izlaz iz admin menija.\n");
                break;
            default:
                printf("Nepoznata opcija.\n");
        }
    } while (izbor != 0);
}



void registracijaKorisnika(void) 
{
    char ime[50], prezime[50], email[50], tip[20], lozinka[50], lozinka2[50];
    char brojTelefona[20], adresa[100], datumRodjenja[15];
    char potvrda, odustani;
    FILE *f;
    char linija[300];
    int vecPostoji = 0;

    printf("=== Registracija korisnika ===\n");

    // Unos imena i prezimena
    printf("Unesite ime: ");
    scanf("%49s", ime);

    printf("Unesite prezime: ");
    scanf("%49s", prezime);

    // Unos tipa korisnika
    do {
        printf("Unesite tip korisnika (ucenik/roditelj/nastavnik/admin): ");
        scanf("%19s", tip);

        // Provjera validnosti tipa
        for(int i=0; tip[i]; i++) tip[i] = tolower(tip[i]);
    } while (strcmp(tip,"ucenik") !=0 && strcmp(tip,"roditelj") !=0 &&
             strcmp(tip,"nastavnik") !=0 && strcmp(tip,"admin") !=0);

    // Unos email-a i provjera da li vec postoji
    do {
        printf("Unesite email: ");
        scanf("%49s", email);

        vecPostoji = 0;
        f = fopen("korisnici.txt", "r");
        if (f != NULL) {
            while (fgets(linija, sizeof(linija), f) != NULL) {
                if (strstr(linija, email) != NULL) {
                    vecPostoji = 1;
                    break;
                }
            }
            fclose(f);
        }

        if (vecPostoji) {
            printf("Korisnik sa ovim email-om vec postoji!\n");
            printf("Da li zelite da unesete drugi email? (Y/N): ");
            scanf(" %c", &odustani);
            if (odustani != 'Y' && odustani != 'y') {
                printf("Registracija prekinuta.\n");
                return;
            }
        }
    } while (vecPostoji);

    // Unos ostalih podataka
    printf("Unesite broj telefona: ");
    scanf("%19s", brojTelefona);

    printf("Unesite adresu stanovanja: ");
    scanf(" %[^\n]%*c", adresa); // cita cijeli red sa razmacima

    printf("Unesite datum rodjenja (dd.mm.yyyy): ");
    scanf("%14s", datumRodjenja);

    // Unos lozinke i potvrda
    do {
        printf("Unesite lozinku: ");
        scanf("%49s", lozinka);

        printf("Ponovite lozinku: ");
        scanf("%49s", lozinka2);

        if (strcmp(lozinka, lozinka2) != 0) {
            printf("Lozinke se ne poklapaju. Pokusajte ponovo.\n");
        }
    } while (strcmp(lozinka, lozinka2) != 0);

    // Prikaz podataka i potvrda registracije
    printf("\nUneseni podaci:\nIme: %s\nPrezime: %s\nTip: %s\nEmail: %s\nBroj telefona: %s\nAdresa: %s\nDatum rodjenja: %s\n",
           ime, prezime, tip, email, brojTelefona, adresa, datumRodjenja);

    printf("Potvrdite registraciju (Y/N): ");
    scanf(" %c", &potvrda);

    if (potvrda != 'Y' && potvrda != 'y') {
        printf("Registracija prekinuta.\n");
        return;
    }

    // Upis u fajl
    f = fopen("korisnici.txt", "a");
    if (f == NULL) {
        printf("Greska pri otvaranju baze korisnika!\n");
        return;
    }

    fprintf(f, "Ime: %s | Prezime: %s | Tip: %s | Email: %s | Lozinka: %s | Broj telefona: %s | Adresa: %s | Datum rodjenja: %s\n",
            ime, prezime, tip, email, lozinka, brojTelefona, adresa, datumRodjenja);
    fclose(f);

    printf("Korisnik uspjesno registrovan!\n");
}

void obrisiKorisnika()
{
    FILE *f = fopen("korisnici.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    FILE *arhiva = fopen("arhiva_obrisanih.txt", "a");

    if (!f || !temp || !arhiva) {
        printf("Greska pri radu sa fajlovima.\n");
        return;
    }

    char email[50];
    char linija[600];
    char potvrda;
    int pronadjen = 0;

    printf("Unesite email korisnika za brisanje: ");
    scanf("%49s", email);

    while (fgets(linija, sizeof(linija), f)) {
        if (strstr(linija, email)) {
            pronadjen = 1;

            printf("Pronadjen korisnik:\n%s", linija);
            printf("Da li ste sigurni da zelite trajno obrisati ovaj nalog? (Y/N): ");
            scanf(" %c", &potvrda);

            if (potvrda == 'Y' || potvrda == 'y') {
                // Arhiviranje obrisanog korisnika
                fprintf(arhiva, "%s", linija);
                printf("Korisnicki nalog uspjesno obrisan i arhiviran.\n");
            } else {
                // Ako administrator odustane, korisnik ostaje
                fputs(linija, temp);
                printf("Brisanje otkazano.\n");
            }
        } else {
            fputs(linija, temp);
        }
    }

    fclose(f);
    fclose(temp);
    fclose(arhiva);

    remove("korisnici.txt");
    rename("temp.txt", "korisnici.txt");

    if (!pronadjen) {
        printf("Korisnicki nalog nije pronadjen ili je vec obrisan.\n");
    }
}
