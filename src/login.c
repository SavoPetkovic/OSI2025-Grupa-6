#include <stdio.h>
#include <string.h>
#include "login.h"

// Funkcija koja uklanja vodece i pratece razmake iz stringa
void trim(char *str) 
{
    int i;
    // ukloni trailing spaces
    i = strlen(str) - 1;
    while(i >= 0 && (str[i] == ' ' || str[i] == '\t' || str[i]=='\n')) {
        str[i] = '\0';
        i--;
    }
    // ukloni leading spaces
    while(*str == ' ' || *str == '\t') {
        memmove(str, str+1, strlen(str));
    }
}

int prijavaKorisnika(char role[20]) 
{
    char email[50], lozinka[50], linija[500];
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
                // Provjera statusa
                char *statusP = strstr(linija, "Status:");
                if (statusP) {
                    statusP += 7; // preskoci "Status:"
                    char *kraj = strchr(statusP, '|'); 
                    char status[20];
                    if (kraj) {
                        *kraj = '\0';
                        strcpy(status, statusP);
                    } else {
                        strcpy(status, statusP);
                    }
                    trim(status);
                    if (strcmp(status, "blokiran") == 0) {
                        printf("Nalog je blokiran. Prijava nije moguca.\n");
                        uspjesno = 0;
                        break;
                    }
                }

                // Ekstrakcija tipa korisnika
                char *p = strstr(linija, "Tip: ");
                if (p) {
                    p += 5; // preskoci "Tip: "
                    char *kraj = strchr(p, '|');
                    if (kraj) *kraj = '\0'; // prekini string na |
                    trim(p);               // ukloni razmake sa pocetka i kraja
                    strcpy(role, p);
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
