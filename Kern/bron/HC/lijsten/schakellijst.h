#ifndef SCHAKELLIJST_H
#define SCHAKELLIJST_H

#include "HC/algemeen.h"

/**
 * @file
 * @short Een schakellijst.
 *
 * Gebruikt opeenvolgende schakels voor alles wat toegevoegd wordt.
 */

typedef struct schakel Schakel;
struct schakel {
	Schakel* volgende;
	char inhoud[];
};

typedef struct schakellijst SchakelLijst;
struct schakellijst {
	unsigned int tel;
	size_t onderdeel_grootte;
	Schakel* begin;
	Schakel* einde;
};

SchakelLijst* maakSchakelLijst(size_t onderdeel_grootte);

void schakellijstVoeg(SchakelLijst* lijst, void* onderdeel);
// TODO void slijstVoegIn(SchakelLijst* lijst, unsigned int plek, void* onderdeel);

#define schakellijstKrijg(lijst, plek, soort) (*(*soort)(schakellijstKrijg_v(lijst, plek)->inhoud))
booleaan schakellijstVind(SchakelLijst* lijst, const void* onderdeel, vergelijk_opdracht vergelijker, unsigned int* plek);

void schakellijstVerwijder(SchakelLijst* lijst, unsigned int plek, verwijder_opdracht opdracht);
booleaan schakellijstVindVerwijder(SchakelLijst* lijst, void* onderdeel, vergelijk_opdracht vergelijker, verwijder_opdracht opdracht);

void verwijderSchakelLijst(SchakelLijst* lijst, verwijder_opdracht opdracht);

// TODO voor elk & print

#define voor_elk_schakel(lijst, i, soort)                                         \
	Schakel* _schakel;                                                            \
	soort* i;                                                                     \
	for (_schakel = lijst->begin, i = (soort*)_schakel->inhoud; _schakel != NULL; \
		 _schakel = _schakel->volgende, i = (soort*)_schakel->inhoud)

#endif