#include <stdio.h>
#include <string.h>
#include "login.h"
#include "roditelj.h"
#include "ucenik.h"

int main()
{
    char uloga[20];

    if (prijavaKorisnika(uloga)) {

        if (strcmp(uloga, "roditelj") == 0){
            roditeljMeni();
        }  
        else if (strcmp(uloga, "ucenik") == 0) {
            ucenikMeni();
        }
        else {
            printf("Nemate pristup ovom programu.\n");
        }
    }
    return 0;
}
