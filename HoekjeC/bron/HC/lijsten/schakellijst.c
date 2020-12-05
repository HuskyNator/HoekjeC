#include "schakellijst.h"

#include <stdio.h>
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

void schakellijstVoeg(SchakelLijst* lijst, const void* onderdeel) {
	Schakel* schakel = malloc(sizeof(Schakel) + lijst->onderdeel_grootte);
	memcpy(schakel->inhoud, onderdeel, lijst->onderdeel_grootte);
	schakel->volgende = NULL;

	if (lijst->begin == NULL) lijst->begin = schakel;
	else
		lijst->einde->volgende = schakel;
	lijst->einde = schakel;

	lijst->tel++;
}

Schakel* schakellijstKrijg_schakel(SchakelLijst* lijst, unsigned int plek) {
	if (plek >= lijst->tel) return NULL;
	if (plek == lijst->tel - 1) return lijst->einde;
	Schakel* zoek_plek = lijst->begin;
	for (unsigned int i = 0; i < plek; i++) {
		zoek_plek = zoek_plek->volgende;
	}
	return zoek_plek;
}

booleaan schakellijstVind(SchakelLijst* lijst, const void* onderdeel, vergelijk_opdracht vergelijker, unsigned int* plek) {
	unsigned int i_plek = 0;
	schakellijstLus(lijst, i, void*) {
		if (vergelijker(i, onderdeel, lijst->onderdeel_grootte)) {
			if (plek != NULL) *plek = i_plek;
			return waar;
		}
		i_plek++;
	}
	return onwaar;
}

void schakellijstVerwijder(SchakelLijst* lijst, unsigned int plek, verwijder_opdracht opdracht) {
	if (plek >= lijst->tel) return;

	Schakel *schakel, *vorige;
	vorige = schakellijstKrijg_schakel(lijst, plek - 1);
	if (vorige == NULL) {
		schakel = lijst->begin;
		lijst->begin = schakel->volgende;
	} else {
		schakel = vorige->volgende;
		vorige->volgende = schakel->volgende;
	}
	if (schakel->volgende == NULL) lijst->einde = vorige;

	if (opdracht != NULL) opdracht(schakel->inhoud);

	lijst->tel--;
	free(schakel);
}

booleaan schakellijstVindVerwijder(SchakelLijst* lijst, const void* onderdeel, vergelijk_opdracht vergelijker,
								   verwijder_opdracht opdracht) {
	unsigned int plek;
	if (!schakellijstVind(lijst, onderdeel, vergelijker, &plek)) return onwaar;
	schakellijstVerwijder(lijst, plek, opdracht);
	return waar;
}

void schakellijstAfdrukken(SchakelLijst* lijst, afdruk_opdracht opdracht) {
	putchar('<');
	Schakel* teller = lijst->begin;
	while (teller != lijst->einde) {
		opdracht(teller->inhoud);
		putchar(' ');
		putchar(',');
		teller = teller->volgende;
	}
	opdracht(teller->inhoud);
	putchar('>');
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
