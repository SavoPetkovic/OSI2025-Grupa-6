#ifndef LOGIN_H
#define LOGIN_H

// Funkcija prijave korisnika
// role - string koji ce biti popunjen tipom korisnika (ucenik, nastavnik, admin, roditelj)
// Vraca: 1 = uspjesna prijava, 0 = neuspesna
int prijavaKorisnika(char role[20]);

#endif
