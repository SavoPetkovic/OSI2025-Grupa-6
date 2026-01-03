#include <stdio.h>
#include <string.h>
#include "login.h"

int prijavaKorisnika(char role[20]) {
    char email[50], lozinka[50], linija[300];
    FILE *f;
    int pokusaji = 0;
    int uspjesno = 0;

    printf("=== Prijava korisnika ===\n");

    while (pokusaji < 3 && !uspjesno) {
        printf("Unesite email: ");
        scanf("%49s", email);

        printf("Unesite lozinku: ");
        scanf("%49s", lozinka);

        // Otvaranje baze korisnika
        f = fopen("korisnici.txt", "r");
        if (f == NULL) {
            printf("Greska: baza korisnika nije dostupna!\n");
            return 0;
        }

        uspjesno = 0;
        while (fgets(linija, sizeof(linija), f)) {
            // Provjera emaila i lozinke
            if (strstr(linija, email) && strstr(linija, lozinka)) {
                // Pronadji tip korisnika
                char *ptr = strstr(linija, "Tip: ");
                if (ptr != NULL) {
                    ptr += 5; // preskoci "Tip: "
                    int i = 0;
                    while (ptr[i] != '|' && ptr[i] != '\n' && i < 19) {
                        role[i] = ptr[i];
                        i++;
                    }
                    role[i] = '\0';
                } else {
                    strcpy(role, "Nepoznata");
                }
                uspjesno = 1;
                break;
            }
        }

        fclose(f);

        if (!uspjesno) {
            printf("Pogresan email ili lozinka. Pokusajte ponovo.\n");
            pokusaji++;
        }
    }

    if (uspjesno) {
        printf("Uspjesna prijava! Vas tip korisnika: %s\n", role);
        return 1;
    } else {
        printf("Previse neuspjesnih pokusaja. Prijava nije moguca.\n");
        return 0;
    }
}
