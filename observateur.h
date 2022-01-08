#include <stdlib.h>
#include <stdio.h>

typedef enum {
    A,
    M,
    G,
    D
}Alphabet;

typedef enum {
    INIT,
    MES,
    ERR
}Etat;

Etat transition(Etat e, Alphabet alp);

Etat initialiser_etat(Etat e);