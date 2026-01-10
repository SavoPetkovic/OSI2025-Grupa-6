#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "admin.h"
#include "raspored.h"
#include "nastavnik.h"


void blokirajKorisnika() 
{
    FILE *f = fopen("korisnici.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!f || !temp) {
        printf("Greska pri radu sa fajlovima.\n");
        return;
    }

    char email[50];
    char razlog[100];
    char linija[600];
    int pronadjen = 0;

    printf("Unesite email korisnika za blokiranje: ");
    scanf("%49s", email);
    getchar(); 

    printf("Unesite razlog blokiranja: ");
    fgets(razlog, sizeof(razlog), stdin);
    razlog[strcspn(razlog, "\n")] = 0; // uklanja \n sa fgets

    while (fgets(linija, sizeof(linija), f)) {
        if (strstr(linija, email)) {
            pronadjen = 1;

            if (strstr(linija, "Status: blokiran")) {
                printf("Nalog je vec blokiran.\n");
                fputs(linija, temp);
            } else {
                
                char ime[50], prezime[50], tip[20], mail[50], lozinka[50];
                char broj[20], adresa[100], datum[20];

                sscanf(linija,
                    "Ime: %49[^|]| Prezime: %49[^|]| Tip: %19[^|]| Email: %49[^|]| Lozinka: %49[^|]| Broj telefona: %19[^|]| Adresa: %99[^|]| Datum rodjenja: %19[^\n]",
                    ime, prezime, tip, mail, lozinka, broj, adresa, datum);

               
                char novaLinija[800];
                sprintf(novaLinija,
                    "Ime: %s | Prezime: %s | Tip: %s | Email: %s | Lozinka: %s | Broj telefona: %s | Adresa: %s | Datum rodjenja: %s | Status: blokiran | Razlog: %s\n",
                    ime, prezime, tip, mail, lozinka, broj, adresa, datum, razlog);

                fputs(novaLinija, temp);
                printf("Korisnik uspjesno blokiran.\n");
            }
        } else {
            fputs(linija, temp);
        }
    }

    fclose(f);
    fclose(temp);

    remove("korisnici.txt");
    rename("temp.txt", "korisnici.txt");

    if (!pronadjen) {
        printf("Korisnik nije pronadjen.\n");
    }
}


void adminMeni() 
{
    int izbor;

    do {
        printf("\n=== ADMIN MENI ===\n");
        printf("1. Registracija korisnika\n");
        printf("2. Blokiranje korisnika\n");
        printf("3. Brisanje korisnickog naloga\n");
        printf("4. Kreiranje predmeta\n");
        printf("5. Kreiranje odjeljenja\n");
        printf("6. Dodavanje ucenika u odjeljenje\n");
        printf("7. Pregled nastavnika\n");
        printf("8. Definisanje vrsta ocjena\n");
        printf("9. Pregled izostanaka (read-only)\n");
        printf("10. Pregled svih ocjena\n");
        printf("11. Pregled rasporeda casova\n");
        printf("12. Generisanje izvjestaja o uspjehu ucenika\n");
        printf("0. Izlaz\n");
        printf("Izbor: ");
        scanf("%d", &izbor);

        switch (izbor) {

            case 1:
                registracijaKorisnika();
                break;

            case 2:
                blokirajKorisnika();
                break;

            case 3:
                obrisiKorisnika();
                break;

            case 4:
                kreirajPredmet();
                break;

            case 5:
                kreirajOdjeljenje();
                break;
            case 6:
                dodajUcenikaUOdjeljenje();
                break;

            case 7:
                pregledNastavnika();
                break;

            case 8:
                definisanjeVrstaOcjena();
                break;

            case 9:
                pregledIzostanaka(); 
                break;
            case 10:
                pregledSvihOcjena();
                break;
            case 11:
                pregledRasporeda();
                break;
            case 12: 
                generisanjeIzvjestajUspjehaUcenika();
                break;
            case 0:
                printf("Izlaz iz admin menija.\n");
                break;
            default:
                printf("Nepoznata opcija.\n");
        }

    } while (izbor != 0);
}

void registracijaKorisnika(void) 
{
    char ime[50], prezime[50], email[50], tip[20], lozinka[50], lozinka2[50];
    char brojTelefona[20], adresa[100], datumRodjenja[15];
    char potvrda, odustani;
    FILE *f;
    char linija[300];
    int vecPostoji = 0;

    printf("=== Registracija korisnika ===\n");

    
    printf("Unesite ime: ");
    scanf("%49s", ime);

    printf("Unesite prezime: ");
    scanf("%49s", prezime);

    
    do {
        printf("Unesite tip korisnika (ucenik/roditelj/nastavnik/admin): ");
        scanf("%19s", tip);

        // Provjera validnosti tipa
        for(int i=0; tip[i]; i++) tip[i] = tolower(tip[i]);
    } while (strcmp(tip,"ucenik") !=0 && strcmp(tip,"roditelj") !=0 &&
             strcmp(tip,"nastavnik") !=0 && strcmp(tip,"admin") !=0);

    
    do {
        printf("Unesite email: ");
        scanf("%49s", email);

        vecPostoji = 0;
        f = fopen("korisnici.txt", "r");
        if (f != NULL) {
            while (fgets(linija, sizeof(linija), f) != NULL) {
                if (strstr(linija, email) != NULL) {
                    vecPostoji = 1;
                    break;
                }
            }
            fclose(f);
        }

        if (vecPostoji) {
            printf("Korisnik sa ovim email-om vec postoji!\n");
            printf("Da li zelite da unesete drugi email? (Y/N): ");
            scanf(" %c", &odustani);
            if (odustani != 'Y' && odustani != 'y') {
                printf("Registracija prekinuta.\n");
                return;
            }
        }
    } while (vecPostoji);

    
    printf("Unesite broj telefona: ");
    scanf("%19s", brojTelefona);

    printf("Unesite adresu stanovanja: ");
    scanf(" %[^\n]%*c", adresa); // cita cijeli red sa razmacima

    printf("Unesite datum rodjenja (dd.mm.yyyy): ");
    scanf("%14s", datumRodjenja);

    
    do {
        printf("Unesite lozinku: ");
        scanf("%49s", lozinka);

        printf("Ponovite lozinku: ");
        scanf("%49s", lozinka2);

        if (strcmp(lozinka, lozinka2) != 0) {
            printf("Lozinke se ne poklapaju. Pokusajte ponovo.\n");
        }
    } while (strcmp(lozinka, lozinka2) != 0);

    
    printf("\nUneseni podaci:\nIme: %s\nPrezime: %s\nTip: %s\nEmail: %s\nBroj telefona: %s\nAdresa: %s\nDatum rodjenja: %s\n",
           ime, prezime, tip, email, brojTelefona, adresa, datumRodjenja);

    printf("Potvrdite registraciju (Y/N): ");
    scanf(" %c", &potvrda);

    if (potvrda != 'Y' && potvrda != 'y') {
        printf("Registracija prekinuta.\n");
        return;
    }

    // Upis u fajl
    f = fopen("korisnici.txt", "a");
    if (f == NULL) {
        printf("Greska pri otvaranju baze korisnika!\n");
        return;
    }

    fprintf(f, "Ime: %s | Prezime: %s | Tip: %s | Email: %s | Lozinka: %s | Broj telefona: %s | Adresa: %s | Datum rodjenja: %s\n",
            ime, prezime, tip, email, lozinka, brojTelefona, adresa, datumRodjenja);
    fclose(f);

    printf("Korisnik uspjesno registrovan!\n");
}

void obrisiKorisnika()
{
    FILE *f = fopen("korisnici.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    FILE *arhiva = fopen("arhiva_obrisanih.txt", "a");

    if (!f || !temp || !arhiva) {
        printf("Greska pri radu sa fajlovima.\n");
        return;
    }

    char email[50];
    char linija[600];
    char potvrda;
    int pronadjen = 0;

    printf("Unesite email korisnika za brisanje: ");
    scanf("%49s", email);

    while (fgets(linija, sizeof(linija), f)) {
        if (strstr(linija, email)) {
            pronadjen = 1;

            printf("Pronadjen korisnik:\n%s", linija);
            printf("Da li ste sigurni da zelite trajno obrisati ovaj nalog? (Y/N): ");
            scanf(" %c", &potvrda);

            if (potvrda == 'Y' || potvrda == 'y') {
                // Arhiviranje obrisanog korisnika
                fprintf(arhiva, "%s", linija);
                printf("Korisnicki nalog uspjesno obrisan i arhiviran.\n");
            } else {
                // Ako administrator odustane, korisnik ostaje
                fputs(linija, temp);
                printf("Brisanje otkazano.\n");
            }
        } else {
            fputs(linija, temp);
        }
    }

    fclose(f);
    fclose(temp);
    fclose(arhiva);

    remove("korisnici.txt");
    rename("temp.txt", "korisnici.txt");

    if (!pronadjen) {
        printf("Korisnicki nalog nije pronadjen ili je vec obrisan.\n");
    }
}
void kreirajPredmet()
{
    FILE *f;
    char naziv[50], sifra[20], nastavnik[50];
    int razred, brojCasova;
    char emailNastavnika[50];
    char linija[300];
    int postoji = 0;
    char potvrda;

    printf("=== Kreiranje novog predmeta ===\n");

    printf("Unesite naziv predmeta: ");
    scanf(" %[^\n]%*c", naziv);

    
    f = fopen("predmeti.txt", "r");
    if (f != NULL) {
        while (fgets(linija, sizeof(linija), f)) {
            if (strstr(linija, naziv)) {
                postoji = 1;
                break;
            }
        }
        fclose(f);
    }

    if (postoji) {
        printf("Predmet sa ovim nazivom vec postoji!\n");
        return;
    }

    printf("Unesite sifru predmeta: ");
    scanf("%19s", sifra);

    printf("Unesite razred: ");
    scanf("%d", &razred);

    printf("Unesite broj casova sedmicno: ");
    scanf("%d", &brojCasova);

    printf("Unesite ime nastavnika: ");
    scanf(" %[^\n]%*c", nastavnik);

    printf("Unesite email nastavnika: ");
    scanf("%49s", emailNastavnika);


    // Potvrda
    printf("\nUneseni podaci:\n");
    printf("Naziv: %s\nSifra: %s\nRazred: %d\nBroj casova: %d\nNastavnik: %s\nEmail: %s\n",
           naziv, sifra, razred, brojCasova, nastavnik,emailNastavnika);

    printf("Potvrdite kreiranje predmeta (Y/N): ");
    scanf(" %c", &potvrda);

    if (potvrda != 'Y' && potvrda != 'y') {
        printf("Kreiranje predmeta otkazano.\n");
        return;
    }

    // Upis u fajl
    f = fopen("predmeti.txt", "a");
    if (f == NULL) {
        printf("Greska pri otvaranju fajla predmeti.txt\n");
        return;
    }

    fprintf(f,
    "Naziv: %s | Sifra: %s | Razred: %d | BrojCasova: %d | Nastavnik: %s | Email: %s\n",
     naziv, sifra, razred, brojCasova, nastavnik, emailNastavnika);


    fclose(f);

    printf("Predmet uspjesno kreiran!\n");
}
void kreirajOdjeljenje()
{
    FILE *f;
    char oznaka[10], skolskaGodina[20], razredni[50];
    int razred;
    char linija[300];
    int postoji = 0, zauzetRazrednik = 0;
    char potvrda;

    printf("=== Kreiranje novog odjeljenja ===\n");

    printf("Unesite razred (npr. 1, 2, 3...): ");
    scanf("%d", &razred);

    printf("Unesite oznaku odjeljenja (npr. A, B): ");
    scanf("%9s", oznaka);

    printf("Unesite skolsku godinu (npr. 2024/2025): ");
    scanf("%19s", skolskaGodina);

    printf("Unesite ime razrednog starjesine: ");
    scanf(" %[^\n]%*c", razredni);

    
    f = fopen("odjeljenja.txt", "r");
    if (f != NULL) {
        while (fgets(linija, sizeof(linija), f)) {

          
            if (strstr(linija, oznaka) &&
                strstr(linija, skolskaGodina)) {

                int r;
                sscanf(linija, "Razred: %d", &r);
                if (r == razred) {
                    postoji = 1;
                    break;
                }
            }

       
            if (strstr(linija, razredni)) {
                zauzetRazrednik = 1;
            }
        }
        fclose(f);
    }

    if (postoji) {
        printf("Greska: Ovo odjeljenje vec postoji!\n");
        return;
    }

    if (zauzetRazrednik) {
        printf("Greska: Nastavnik je vec razredni starjesina drugog odjeljenja!\n");
        return;
    }

   
    printf("\nUneseni podaci:\n");
    printf("Razred: %d\n", razred);
    printf("Oznaka: %s\n", oznaka);
    printf("Skolska godina: %s\n", skolskaGodina);
    printf("Razredni starjesina: %s\n", razredni);

    printf("Potvrdite kreiranje odjeljenja (Y/N): ");
    scanf(" %c", &potvrda);

    if (potvrda != 'Y' && potvrda != 'y') {
        printf("Kreiranje odjeljenja otkazano.\n");
        return;
    }

    /* Upis u fajl */
    f = fopen("odjeljenja.txt", "a");
    if (f == NULL) {
        printf("Greska pri otvaranju fajla odjeljenja.txt\n");
        return;
    }

    fprintf(f,
        "Razred: %d | Oznaka: %s | Skolska godina: %s | Razredni starjesina: %s\n",
        razred, oznaka, skolskaGodina, razredni);

    fclose(f);

    printf("Odjeljenje uspjesno kreirano!\n");
}
void dodajUcenikaUOdjeljenje()
{
    FILE *fKorisnici, *fOdjeljenja, *fVeza;
    char ime[50], prezime[50], email[50];
    char razred[5], oznaka[5];
    char linija[300];
    int postojiUcenik = 0, postojiOdjeljenje = 0, vecUBazi = 0;
    char potvrda;

    printf("\n=== DODAVANJE UCENIKA U ODJELJENJE ===\n");

    
    printf("Ime ucenika: ");
    scanf("%49s", ime);

    printf("Prezime ucenika: ");
    scanf("%49s", prezime);

    printf("Email ucenika: ");
    scanf("%49s", email);

    
    fKorisnici = fopen("korisnici.txt", "r");
    if (!fKorisnici) {
        printf("Greska pri otvaranju korisnici.txt\n");
        return;
    }

    while (fgets(linija, sizeof(linija), fKorisnici)) {
        if (strstr(linija, "Tip: ucenik") && strstr(linija, email)) {
            postojiUcenik = 1;
            break;
        }
    }
    fclose(fKorisnici);

    if (!postojiUcenik) {
        printf("Greska: Ucenik ne postoji u sistemu!\n");
        return;
    }

    
    printf("Razred (npr. 2): ");
    scanf("%4s", razred);

    printf("Oznaka odjeljenja (npr. A): ");
    scanf("%4s", oznaka);

    
    fOdjeljenja = fopen("odjeljenja.txt", "r");
    if (!fOdjeljenja) {
        printf("Greska pri otvaranju odjeljenja.txt\n");
        return;
    }

    while (fgets(linija, sizeof(linija), fOdjeljenja)) {
        if (strstr(linija, razred) && strstr(linija, oznaka)) {
            postojiOdjeljenje = 1;
            break;
        }
    }
    fclose(fOdjeljenja);

    if (!postojiOdjeljenje) {
        printf("Greska: Odjeljenje ne postoji!\n");
        return;
    }

    
    fVeza = fopen("ucenici_odjeljenja.txt", "r");
    if (fVeza) {
        while (fgets(linija, sizeof(linija), fVeza)) {
            if (strstr(linija, email) &&
                strstr(linija, razred) &&
                strstr(linija, oznaka)) {
                vecUBazi = 1;
                break;
            }
        }
        fclose(fVeza);
    }

    if (vecUBazi) {
        printf("Ucenik je vec u ovom odjeljenju!\n");
        return;
    }

    
    printf("\nDodati ucenika %s %s u odjeljenje %s%s? (Y/N): ",
           ime, prezime, razred, oznaka);
    scanf(" %c", &potvrda);

    if (potvrda != 'Y' && potvrda != 'y') {
        printf("Proces prekinut.\n");
        return;
    }

    /* Upis veze */
    fVeza = fopen("ucenici_odjeljenja.txt", "a");
    if (!fVeza) {
        printf("Greska pri upisu u bazu.\n");
        return;
    }

    fprintf(fVeza,
        "Ucenik: %s %s | Email: %s | Odjeljenje: %s%s\n",
        ime, prezime, email, razred, oznaka);

    fclose(fVeza);

    printf("Ucenik je USPJESNO dodat u odjeljenje!\n");
}

void definisanjeVrstaOcjena()
{
    FILE *f;
    char linija[100];
    char izbor;
    char naziv[50];
    int postoji;

    do {
        printf("\n=== DEFINISANJE VRSTA OCJENA ===\n");

        // Prikaz postojećih vrsta ocjena
        f = fopen("vrste_ocjena.txt", "r");
        if (f != NULL) {
            printf("\nPostojece vrste ocjena:\n");
            while (fgets(linija, sizeof(linija), f)) {
                printf("- %s", linija);
            }
            fclose(f);
        } else {
            printf("Nema definisanih vrsta ocjena.\n");
        }

        printf("\nOpcije:\n");
        printf("1. Dodaj novu vrstu ocjene\n");
        printf("2. Izmijeni postojecu vrstu ocjene\n");
        printf("3. Obrisi vrstu ocjene\n");
        printf("0. Povratak u admin meni\n");
        printf("Izbor: ");
        scanf(" %c", &izbor);
        getchar(); // uklanja \n sa ulaznog bafera

        if (izbor == '1') {
            // Dodavanje nove vrste ocjene
            printf("Unesite naziv nove vrste ocjene: ");
            fgets(naziv, sizeof(naziv), stdin);
            naziv[strcspn(naziv, "\n")] = 0;

            // Provjera da li vec postoji
            postoji = 0;
            f = fopen("vrste_ocjena.txt", "r");
            if (f != NULL) {
                while (fgets(linija, sizeof(linija), f)) {
                    linija[strcspn(linija, "\n")] = 0;
                    if (strcmp(linija, naziv) == 0) {
                        postoji = 1;
                        break;
                    }
                }
                fclose(f);
            }

            if (postoji) {
                printf("Ova vrsta ocjene vec postoji!\n");
            } else {
                f = fopen("vrste_ocjena.txt", "a");
                if (f != NULL) {
                    fprintf(f, "%s\n", naziv);
                    fclose(f);
                    printf("Vrsta ocjene uspjesno dodana!\n");
                } else {
                    printf("Greska pri otvaranju fajla vrste_ocjena.txt\n");
                }
            }
        }
        else if (izbor == '2') {
            // Izmjena postojeće vrste ocjene
            char stariNaziv[50], noviNaziv[50];
            printf("Unesite naziv vrste ocjene koju zelite izmijeniti: ");
            fgets(stariNaziv, sizeof(stariNaziv), stdin);
            stariNaziv[strcspn(stariNaziv, "\n")] = 0;

            f = fopen("vrste_ocjena.txt", "r");
            FILE *temp = fopen("temp.txt", "w");
            int pronadjen = 0;

            if (f == NULL || temp == NULL) {
                printf("Greska pri radu sa fajlovima.\n");
            } else {
                while (fgets(linija, sizeof(linija), f)) {
                    linija[strcspn(linija, "\n")] = 0;
                    if (strcmp(linija, stariNaziv) == 0) {
                        pronadjen = 1;
                        printf("Unesite novi naziv za ovu vrstu ocjene: ");
                        fgets(noviNaziv, sizeof(noviNaziv), stdin);
                        noviNaziv[strcspn(noviNaziv, "\n")] = 0;
                        fprintf(temp, "%s\n", noviNaziv);
                    } else {
                        fprintf(temp, "%s\n", linija);
                    }
                }
                fclose(f);
                fclose(temp);

                if (pronadjen) {
                    remove("vrste_ocjena.txt");
                    rename("temp.txt", "vrste_ocjena.txt");
                    printf("Vrsta ocjene uspjesno izmijenjena.\n");
                } else {
                    remove("temp.txt");
                    printf("Vrsta ocjene nije pronadjena.\n");
                }
            }
        }
        else if (izbor == '3') {
            // Brisanje vrste ocjene
            char brisiNaziv[50];
            printf("Unesite naziv vrste ocjene koju zelite obrisati: ");
            fgets(brisiNaziv, sizeof(brisiNaziv), stdin);
            brisiNaziv[strcspn(brisiNaziv, "\n")] = 0;

            f = fopen("vrste_ocjena.txt", "r");
            FILE *temp = fopen("temp.txt", "w");
            int pronadjen = 0;

            if (f == NULL || temp == NULL) {
                printf("Greska pri radu sa fajlovima.\n");
            } else {
                while (fgets(linija, sizeof(linija), f)) {
                    linija[strcspn(linija, "\n")] = 0;
                    if (strcmp(linija, brisiNaziv) == 0) {
                        pronadjen = 1;
                        continue; // preskoci ovu liniju
                    }
                    fprintf(temp, "%s\n", linija);
                }
                fclose(f);
                fclose(temp);

                if (pronadjen) {
                    remove("vrste_ocjena.txt");
                    rename("temp.txt", "vrste_ocjena.txt");
                    printf("Vrsta ocjene uspjesno obrisana.\n");
                } else {
                    remove("temp.txt");
                    printf("Vrsta ocjene nije pronadjena.\n");
                }
            }
        }

    } while (izbor != '0');
}
void pregledNastavnika()
{
    FILE *f;
    char linija[600];
    char izborEmail[50];
    int imaNastavnika = 0;

    printf("\n=== PREGLED NASTAVNIKA ===\n");

    f = fopen("korisnici.txt", "r");
    if (!f) {
        printf("Greska pri otvaranju korisnici.txt\n");
        return;
    }

    printf("\nLista nastavnika:\n");
    while (fgets(linija, sizeof(linija), f)) {
        if (strstr(linija, "Tip: nastavnik")) {
            char ime[50], prezime[50], email[50];
            sscanf(linija,
                "Ime: %49[^|]| Prezime: %49[^|]| Tip: %*[^|]| Email: %49[^|]",
                ime, prezime, email);

            printf("- %s %s | Email: %s\n", ime, prezime, email);
            imaNastavnika = 1;
        }
    }
    fclose(f);

    if (!imaNastavnika) {
        printf("Nema registrovanih nastavnika.\n");
        return;
    }

    printf("\nUnesite email nastavnika za pregled: ");
    scanf("%49s", izborEmail);

   	f = fopen("predmeti.txt", "r");
		if (!f) {
   		printf("Greska pri otvaranju predmeti.txt\n");
   	 	return;
	}

	printf("\nPredmeti koje nastavnik predaje:\n");
	int imaPredmeta = 0;

	while (fgets(linija, sizeof(linija), f)) 
	{
    		if (strstr(linija, izborEmail)) 
		{
        	printf("- %s", linija);
        	imaPredmeta = 1;
    		}
	}
    fclose(f);

    if (!imaPredmeta) {
        printf("Nema predmeta za ovog nastavnika.\n");
    }

	// NIJE JOS IMPLEMENTIRAN RASPORED JER NIJE URADJENA zahtjev iz specifikacije br. 9 (Pregled rasporeda časova )
    printf("\nOpcije:\n");
    printf("1. Prikaz sedmicnog rasporeda (nije implementirano)\n");
    printf("2. Izmjena rasporeda (nije implementirano)\n");
    printf("0. Povratak u admin meni\n");

    int izbor;
    printf("Izbor: ");
    scanf("%d", &izbor);

    if (izbor == 1) {
        printf("Sedmicni raspored ce biti implementiran kasnije.\n");
    } else if (izbor == 2) {
        printf("Izmjena rasporeda je dodatna funkcionalnost.\n");
    }
}
void pregledIzostanaka()
{
    FILE *f = fopen("izostanci.txt", "r");
    char linija[600];

    printf("\n=== PREGLED IZOSTANAKA (ADMIN) ===\n");

    if (!f) {
        printf("Nema evidentiranih izostanaka.\n");
        return;
    }

    while (fgets(linija, sizeof(linija), f)) {
        printf("%s", linija);
    }

    fclose(f);
}
void pregledSvihOcjena()
{
    FILE *focjene = fopen("ocjene.txt", "r");
    if (!focjene) {
        printf("Greska: ne mogu otvoriti ocjene.txt\n");
        return;
    }

    char ime[50], prezime[50];
    char linija[600];
    int imaOcjena = 0;

    printf("\n=== PREGLED SVIH OCJENA ===\n");

    printf("Ime ucenika: ");
    scanf("%49s", ime);

    printf("Prezime ucenika: ");
    scanf("%49s", prezime);

    printf("\nOcjene za ucenika %s %s:\n", ime, prezime);
    printf("---------------------------------------------\n");

    while (fgets(linija, sizeof(linija), focjene)) {

        if (strstr(linija, ime) && strstr(linija, prezime)) {
            printf("%s", linija);
            imaOcjena = 1;
        }
    }

    if (!imaOcjena) {
        printf("Nema evidentiranih ocjena za ovog ucenika.\n");
    }

    fclose(focjene);
}
