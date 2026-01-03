#include <stdio.h>
#include "admin.h"

void registracijaKorisnika(void) {
    char ime[50], email[50], lozinka[50];
    FILE *f;

    printf("Unesite ime: ");
    scanf("%49s", ime);

    printf("Unesite email: ");
    scanf("%49s", email);

    printf("Unesite lozinku: ");
    scanf("%49s", lozinka);

    f = fopen("korisnici.txt", "a");
    if (f == NULL) { printf("Greska pri otvaranju fajla!\n"); return; }

    fprintf(f, "Ime: %s | Email: %s | Lozinka: %s\n", ime, email, lozinka);
    fclose(f);

    printf("Korisnik uspjesno registrovan!\n");
}
