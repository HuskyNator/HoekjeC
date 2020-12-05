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

/**		MAAK		**/

SchakelLijst* maakSchakelLijst(size_t onderdeel_grootte);

/**		GEBRUIK		**/

/*	Voegen	*/
void schakellijstVoeg(SchakelLijst* lijst, const void* onderdeel);
// TODO void slijstVoegIn(SchakelLijst* lijst, unsigned int plek, void* onderdeel);

/*	Krijgen & Vinden	*/
Schakel* schakellijstKrijg_schakel(SchakelLijst* lijst, unsigned int plek);
#define schakellijstKrijg(lijst, plek, soort) ((soort*)(schakellijstKrijg_schakel(lijst, plek)->inhoud))
booleaan schakellijstVind(SchakelLijst* lijst, const void* onderdeel, vergelijk_opdracht vergelijker, unsigned int* plek);

/*	Verwijderen	*/
void schakellijstVerwijder(SchakelLijst* lijst, unsigned int plek, verwijder_opdracht opdracht);
booleaan schakellijstVindVerwijder(SchakelLijst* lijst, const void* onderdeel, vergelijk_opdracht vergelijker, verwijder_opdracht opdracht);

/*	Lus	*/
#define schakellijstLus(lijst, i, soort)                                          \
	Schakel* _schakel;                                                            \
	soort* i;                                                                     \
	for (_schakel = lijst->begin, i = (soort*)_schakel->inhoud; _schakel != NULL; \
		 _schakel = _schakel->volgende, i = (soort*)_schakel->inhoud)

/*	Afdrukken	*/
void schakellijstAfdrukken(SchakelLijst* lijst, afdruk_opdracht opdracht);

/**		VERWIJDER		**/

void verwijderSchakelLijst(SchakelLijst* lijst, verwijder_opdracht opdracht);

#endif