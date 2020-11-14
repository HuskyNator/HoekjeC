#include "schakellijst.h"

#include <stdlib.h>
#include <string.h>

SchakelLijst* maakSchakelLijst(size_t onderdeel_grootte) {
	SchakelLijst* lijst = malloc(sizeof(SchakelLijst));
	lijst->tel = 0;
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

	lijst->tel++;
}

Schakel* schakellijstKrijg_v(SchakelLijst* lijst, unsigned int plek) {
	if (plek >= lijst->tel) return NULL;
	if (plek == lijst->tel - 1) return lijst->einde->inhoud;
	Schakel* zoek_plek = lijst->begin;
	for (unsigned int i = 0; i < plek; i++) {
		zoek_plek = zoek_plek->volgende;
	}
	return zoek_plek;
}

booleaan schakellijstVind(SchakelLijst* lijst, const void* onderdeel, vergelijk_opdracht vergelijker, unsigned int* plek) {
	unsigned int i_plek = 0;
	voor_elk_schakel(lijst, i, void*) {
		if (vergelijker(i, onderdeel, lijst->onderdeel_grootte)) {
			if (plek != NULL) *plek = i_plek;
			return waar;
		}
		i++;
	}
	return onwaar;
}

void schakellijstVerwijder(SchakelLijst* lijst, unsigned int plek, verwijder_opdracht opdracht) {
	if (plek >= lijst->tel) return;
	Schakel* schakel;
	if (plek == 0) {
		schakel = lijst->begin;
		if (opdracht != NULL) opdracht(schakel->inhoud);

		lijst->begin = schakel->volgende;
		if (lijst->tel == 1) lijst->einde = NULL;
	} else {
		Schakel* vorige = schakellijstKrijg_v(lijst, plek - 1);
		schakel = vorige->volgende;
		if (opdracht != NULL) opdracht(schakel->inhoud);

		vorige->volgende = schakel->volgende;
	}
	lijst->tel--;
	free(schakel);
}

booleaan schakellijstVindVerwijder(SchakelLijst* lijst, void* onderdeel, vergelijk_opdracht vergelijker, verwijder_opdracht opdracht) {
	unsigned int plek;
	if (!schakellijstVind(lijst, onderdeel, vergelijker, &plek)) return onwaar;
	schakellijstVerwijder(lijst, plek, opdracht);
	return waar;
}

void verwijderSchakelLijst(SchakelLijst* lijst, const verwijder_opdracht opdracht) {
	Schakel* huidige = lijst->begin;
	Schakel* volgende;
	for (unsigned int i = 0; i < lijst->tel; i++) {
		volgende = huidige->volgende;
		if (opdracht != NULL) opdracht(huidige->inhoud);
		free(huidige);
		huidige = volgende;
	}
	free(lijst);
}
