#ifndef SNIPPERLIJST_H
#define SNIPPERLIJST_H

#include "schakellijst.h"

/**
 * @file
 * @short Een sleutellijst.
 *
 * Houdt lijsten bij, 'emmers', elk sloten bevattend, sleutel & waarde tweetallen.
 * Tijdens het invoegen van een slot wordt door middel van zijn sleutel door de sleutelaar bepaald tot welke snipper het slot behoort.
 * Op deze manier kan een waarde terug gevonden worden al heeft men haar sleutel.
 */

typedef unsigned int (*sleutel_opdracht)(const void* sleutel);

typedef struct slot Slot;
struct slot {
	void* sleutel;
	void* waarde;
};

typedef struct sleutellijst SleutelLijst;
struct sleutellijst {
	size_t sleutel_grootte;
	size_t waarde_grootte;
	unsigned int emmer_aantal;
	unsigned int tel;
	sleutel_opdracht sleutelaar;
	vergelijk_opdracht vergelijker;
	SchakelLijst* emmers[];
};

/**		MAAK		**/

SleutelLijst* maakSleutelLijst(size_t sleutel_grootte, size_t waarde_grootte, unsigned int emmer_aantal, sleutel_opdracht sleutelaar,
							   vergelijk_opdracht vergelijker);

/**		GEBRUIK		**/

/*	Voegen	*/
void sleutellijstVoeg(SleutelLijst* lijst, const void* sleutel, const void* waarde);

/*	Krijgen	*/
booleaan sleutellijstVind(SleutelLijst* lijst, void* sleutel, void* gevonden_waarde);

/*	Verbeteren	*/
// Probeert de sleutellijst een bezettingsgraad van 75% te geven.
SleutelLijst* sleutellijstVerbeter(SleutelLijst* lijst);

/*	Verwijderen	*/
booleaan sleutellijstVerwijder(SleutelLijst* lijst, void* sleutel, verwijder_opdracht sleutel_opdracht, verwijder_opdracht waarde_opdracht);

/*	Lus	*/
#define sleutellijstLus(lijst, i)                                                                                      \
	unsigned int _emmer;                                                                                               \
	Schakel* _schakel;                                                                                                 \
	Slot* i;                                                                                                           \
	for (_emmer = 0, _schakel = lijst->emmers[0]->begin, i = (Slot*)_schakel->inhoud;                                  \
		 !(_emmer == lijst->emmer_aantal - 1 && _schakel == NULL);                                                     \
		 ((_schakel == NULL) ? (_emmer++, _schakel = lijst->emmers[_emmer]->begin) : (_schakel = _schakel->volgende)), \
		i = (Slot*)_schakel->inhoud)

/*	Afdrukken	*/
void sleutellijstAfdrukken(SleutelLijst* lijst, afdruk_opdracht sleutel_opdracht, afdruk_opdracht waarde_opdracht);

/**		VERWIJDER		**/

void verwijderSleutelLijst(SleutelLijst* lijst, verwijder_opdracht sleutel_opdracht, verwijder_opdracht waarde_opdracht);

#endif