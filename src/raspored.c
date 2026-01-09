#include <stdio.h>
#include <string.h>
#include "raspored.h"

void pregledRasporeda() {
    char razred[5], odjeljenje[5], sedmica[10];
    char linija[600];
    FILE *f;

    printf("\n=== PREGLED RASPOREDA CASOVA ===\n");

    printf("Unesite razred (npr. 2): ");
    scanf("%4s", razred);

    printf("Unesite oznaku odjeljenja (npr. A): ");
    scanf("%4s", odjeljenje);

    printf("Unesite sedmicu (npr. 1): ");
    scanf("%9s", sedmica);

    f = fopen("raspored.txt", "r");
    if (!f) {
        printf("Nema rasporeda za odabrano odjeljenje.\n");
        return;
    }

    int found = 0;
    while (fgets(linija, sizeof(linija), f)) {
        if (strstr(linija, razred) && strstr(linija, odjeljenje) && strstr(linija, sedmica)) {
            printf("%s\n", linija);
            found = 1;
        }
    }

    fclose(f);

    if (!found) {
        printf("Nema rasporeda za odabrani razred/odjeljenje/sedmicu.\n");
    }
}
