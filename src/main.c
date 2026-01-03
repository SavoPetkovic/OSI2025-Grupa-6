#include <stdio.h>
#include <string.h>
#include "admin.h"
#include "login.h"

int main() {
    char uloga[20];

    if (prijavaKorisnika(uloga)) {
        printf("\nDobrodosli! Pristup funkcionalnostima sistema za ulogu: %s\n", uloga);

        // Pozivanje registracije samo za admina
        if (strcmp(uloga, "admin") == 0) {
            registracijaKorisnika();
        } else {
            printf("Ostale funkcionalnosti ce biti implementirane kasnije.\n");
        }
    } else {
        printf("Prijava nije uspjela.\n");
    }

    return 0;
}
