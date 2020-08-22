#include "schakellijst.h"

#include <stdlib.h>
#include <string.h>

SchakelLijst* maakSchakelLijst(size_t onderdeel_grootte) {
	SchakelLijst* lijst = malloc(sizeof(SchakelLijst));
	lijst->tal = 0;
	lijst->onderdeel_grootte = onderdeel_grootte;
	lijst->begin = NULL;
	lijst->einde = NULL;
	return lijst;
}

void schakellijstVoeg(SchakelLijst* lijst, void* onderdeel) {
	Schakel* schakel = malloc(sizeof(Schakel) + lijst->onderdeel_grootte);
	memcpy(schakel->inhoud, onderdeel, lijst->onderdeel_grootte);

	if (lijst->begin == NULL) lijst->begin = schakel;
	else
		lijst->einde->volgende = schakel;
	lijst->einde = schakel;

	lijst->tal++;
}

void* schakellijstKrijg_v(SchakelLijst* lijst, unsigned int plek) {
	if (plek >= lijst->tal) return NULL;
	if (plek == lijst->tal - 1) return lijst->einde->inhoud;
	Schakel* zoek_plek = lijst->begin;
	for (unsigned int i = 0; i < plek; i++) {
		zoek_plek = zoek_plek->volgende;
	}
	return zoek_plek->inhoud;
}

void schakellijstVerwijder(SchakelLijst* lijst, unsigned int plek) {
	if (plek >= lijst->tal) return;
	Schakel* vorige = NULL;
	Schakel* zoek_plek = lijst->begin;
	for (unsigned int i = 0; i < plek; i++) {
		vorige = zoek_plek;
		zoek_plek = zoek_plek->volgende;
	}
	if (vorige != NULL) vorige->volgende = zoek_plek->volgende;
	lijst->tal--;
	free(zoek_plek);
}

void verwijderSchakelLijst(SchakelLijst* lijst) {
	Schakel* huidige = lijst->begin;
	Schakel* volgende;
	for (unsigned int i = 0; i < lijst->tal; i++) {
		volgende = huidige->volgende;
		free(huidige);
		huidige = volgende;
	}
	free(lijst);
}
