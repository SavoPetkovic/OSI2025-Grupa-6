#include <stdio.h>
#include <string.h>
#include "login.h"
#include "roditelj.h"

int main()
{
    char uloga[20];

    if (prijavaKorisnika(uloga)) {

        if (strcmp(uloga, "roditelj") == 0)
            roditeljMeni();
        //potrebno dodati funkcionalnosti za ucenika
        else
            printf("Nemate pristup ovom programu.\n");
    }
    return 0;
}
