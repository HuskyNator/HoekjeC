#ifndef OBJ_LEZER_H
#define OBJ_LEZER_H

#include <stdio.h>
#include "voorwerpen/vorm.h"
#include "lijsten/lijst.h"

Vorm* leesObj(const char* bestandsnaam) {
	FILE* bestand = fopen(bestandsnaam, "rb");
	if (bestand == NULL) {
		fprintf(stderr, "Bestand %s kon niet worden geopend.\n", bestandsnaam);
		return NULL;
	}

	Lijst* hoeken_p = maakLijst;
	Lijst* hoeken_n;
	Lijst* hoeken_v;
}

#endif