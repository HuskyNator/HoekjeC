#include "lezer.h"

#include <stdlib.h>
#include <string.h>

Schrift* leesTot(FILE* bestand, char tot) {
	Schrift* schrift = maakSchrift(NULL);
	char c;
	while (c != EOF && c != tot) {
		c = getc(bestand);
		lijstVoeg(schrift, &c);
	}
	if (c == EOF) {
		verwijderLijst(schrift, onwaar);
		return NULL;
	}
	lijstGroei(schrift, schrift->tel + 1);	// Bespaart geheugen & een krimp.
	lijstVoeg(schrift, &nulteken);
	return schrift;
}

static char witruimte[] = {' ', '\t', '\r', '\n'};
static Lijst witruimte_lijst = (Lijst){.grootte = 4, .tel = 4, .onderdeel_grootte = sizeof(char), .inhoud = witruimte};

static char regeleinde[] = {'\n'};
static Lijst regeleinde_lijst = (Lijst){.grootte = 1, .tel = 1, .onderdeel_grootte = sizeof(char), .inhoud = regeleinde};

char* lezerLeesRegel(FILE* bestand) {
	unsigned int tel = 0;
	unsigned int grootte = 140;
	char* regel = malloc(grootte * sizeof(char));
	char teken = getc(bestand);
	while (teken != '\n' && teken != EOF) {
		if (tel == grootte) regel = realloc(regel, 2 * grootte);
		regel[tel] = teken;
		teken = getc(bestand);
		tel++;
	}
	if (tel == grootte) realloc(regel, grootte + 1);
	regel[tel] = '\0';
	return regel;
}

char* leesWoord(FILE* bestand) {
	unsigned int tel = 0;
	unsigned int grootte = 40;
	char* woord = malloc(grootte * sizeof(char));
	char teken = getc(bestand);
	while (lijstVind(&witruimte_lijst, &teken, geheugen_vergelijker, NULL)) teken = getc(bestand);
	while (teken != EOF & !lijstVind(&witruimte_lijst, &teken, geheugen_vergelijker, NULL)) {
		if (tel == grootte) {
			grootte *= 2;
			woord = realloc(woord, grootte * sizeof(char));
		}
		woord[tel] = teken;
		tel++;
	}
	return woord;
}

booleaan woordBegintMet(char* woord, char* begin) {
	if (begin[0] == '\0') return waar;
	unsigned int tel = 0;
	while (woord[tel] == begin[tel]) {
		tel++;
		if (begin[tel] == '\0') return waar;
	}
	return onwaar;
}

Lijst* woordSplits(char* woord, char teken) {
	Lijst* lijst = maakLijst(5, sizeof(char*));
	unsigned int tel = 0;
	unsigned int begin = 0;
	while (woord[tel] != '\0') {
		if (woord[tel] == teken) {
			unsigned int grootte = tel - begin;
			char* deelwoord = malloc((grootte + 1) * sizeof(char));
			memcpy(deelwoord, &woord[begin], grootte);
			deelwoord[grootte] = '\0';
			lijstVoeg(lijst, &deelwoord);
			begin = tel;
		}
		tel++;
	}
	return lijst;
}