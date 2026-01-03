#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "admin.h"

void registracijaKorisnika(void) {
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
