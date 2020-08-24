#ifndef LEZER_H
#define LEZER_H

#include "lijsten/lijst.h"

#include <stdio.h>

typedef struct lezer Lezer;
struct lezer {
	FILE* bestand;
	Lijst* tekens;	// Vrij aan te passen.
	char* leesruimte;
	unsigned int leesruimte_grootte;
};

Lezer* maakLezer(FILE* bestand);

char* lezerLeesRegel(Lezer* lezer);

char* lezerLeesWoord(Lezer* lezer);

void lezerZetScheidstekens(Lezer* lezer, Lijst* tekens);

#endif