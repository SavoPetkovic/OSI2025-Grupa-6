#include <stdio.h>
#include <string.h>
#include "login.h"

int prijavaKorisnika(char uloga[20])
{
    char email[50], lozinka[50];
    char linija[500];
    FILE *f;
    int pokusaji = 0;

    while (pokusaji < 3) {
        printf("Email: ");
        scanf("%49s", email);

        printf("Lozinka: ");
        scanf("%49s", lozinka);

        f = fopen("korisnici.txt", "r");
        if (!f) {
            printf("Greska: ne mogu otvoriti korisnici.txt\n");
            return 0;
        }

        while (fgets(linija, sizeof(linija), f)) {

            /* tacna provjera emaila i lozinke */
            if (strstr(linija, email) && strstr(linija, lozinka)) {

                /* provjera statusa */
                if (strstr(linija, "Status: blokiran")) {
                    printf("Nalog je blokiran!\n");
                    fclose(f);
                    return 0;
                }

                /* izdvajanje uloge */
                char *p = strstr(linija, "Tip: ");
                if (p) {
                    p += 5;                    // preskoci "Tip: "
                    sscanf(p, " %19[^| ]", uloga);
                } else {
                    strcpy(uloga, "Nepoznata");
                }

                fclose(f);
                return 1;
            }
        }

        fclose(f);
        pokusaji++;
        printf("Pogresan email ili lozinka (%d/3)\n", pokusaji);
    }

    printf("Previse neuspjesnih pokusaja.\n");
    return 0;
}
