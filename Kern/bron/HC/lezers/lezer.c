#include "lezer.h"

#include "lijsten/lijst.h"

#include <stdlib.h>
#include <string.h>

static char witruimte[] = {' ', '\t', '\r', '\n'};
static Lijst witruimte_lijst =
	(Lijst){.grootte = 4, .tel = 4, .onderdeel_grootte = sizeof(char), .inhoud = witruimte};

static char regeleinde[] = {'\n'};
static Lijst regeleinde_lijst =
	(Lijst){.grootte = 1, .tel = 1, .onderdeel_grootte = sizeof(char), .inhoud = regeleinde};

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
	while (lijstVind(witruimte, teken, NULL)) teken = getc(bestand);
	while (teken != EOF & !lijstVind(witruimte, teken, NULL)) {
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
			memcpy(deelwoord, woord[begin], grootte);
			deelwoord[grootte] = '\0';
			lijstVoeg(lijst, &deelwoord);
			begin = tel;
		}
		tel++;
	}
	return lijst;
}