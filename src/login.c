#include <stdio.h>
#include <string.h>
#include "login.h"

int prijavaKorisnika(char uloga[20])
{
    char email[50], lozinka[50];
    char fajlEmail[50], fajlLozinka[50];
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
            printf("Greska: ne mogu otvoriti fajl.\n");
            return 0;
        }

        while (fgets(linija, sizeof(linija), f)) {

            char *e = strstr(linija, "Email: ");
            char *l = strstr(linija, "Lozinka: ");
            char *t = strstr(linija, "Tip: ");

            if (!e || !l || !t) continue;

            sscanf(e + 7, "%49[^|]", fajlEmail);
            sscanf(l + 9, "%49[^|]", fajlLozinka);
            sscanf(t + 5, "%19[^|]", uloga);

            /* uklanjanje razmaka */
            fajlEmail[strcspn(fajlEmail, " \n")] = 0;
            fajlLozinka[strcspn(fajlLozinka, " \n")] = 0;
            uloga[strcspn(uloga, " \n")] = 0;

            if (strcmp(email, fajlEmail) == 0 &&
                strcmp(lozinka, fajlLozinka) == 0) {

                if (strstr(linija, "Status: blokiran")) {
                    printf("Nalog je blokiran!\n");
                    fclose(f);
                    return 0;
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

