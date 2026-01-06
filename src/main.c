
#include <stdio.h>
#include <string.h>
#include "admin.h"
#include "login.h"
#include "nastavnik.h"

int main() {
    char uloga[20];

    if (prijavaKorisnika(uloga)) {
        printf("\nDobrodosli! Pristup funkcionalnostima sistema za ulogu: %s\n", uloga);

        if (strcmp(uloga, "admin") == 0) {
            adminMeni();  
            printf("Ostale funkcionalnosti ce biti implementirane kasnije.\n");
        }
        else if (strcmp(uloga, "nastavnik") == 0) {
            nastavnikMeni();
        }
        else {
            printf("Ova uloga nema pristup ovom programu.\n");
        }
    } else {
        printf("Prijava nije uspjela.\n");
    }

    return 0;
}

