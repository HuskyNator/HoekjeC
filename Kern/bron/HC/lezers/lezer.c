#include "lezer.h"

#include <stdlib.h>

static char standaard_scheidstekens[] = {' ', '\t', '\n', EOF};
static Lijst standaard_scheidstekens_lijst = (Lijst){4, 4, sizeof(char), &standaard_scheidstekens};

Lezer* maakLezer(FILE* bestand) {
	Lezer* lezer = malloc(sizeof(Lezer));
	lezer->bestand = bestand;
	lezer->tekens = &standaard_scheidstekens_lijst;
	lezer->leesruimte = malloc(120 * sizeof(char));
	lezer->leesruimte_grootte = 120;
	return lezer;
}

char* lezerLeesRegel(Lezer* lezer) {
	char teken = getc(lezer->bestand);
	unsigned int tel = 0;
	while (teken != '\n' && teken != EOF) {
		if (tel == lezer->leesruimte_grootte){} lezer->tekens[tel] = teken;
		teken = getc(lezer->bestand);
	}
}

char* lezerLeesWoord(Lezer* lezer) {}
