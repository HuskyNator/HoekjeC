#ifndef SCHAKELLIJST_H
#define SCHAKELLIJST_H

#include "HC/booleaan.h"

#include <stddef.h>

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
	unsigned int tal;
	size_t onderdeel_grootte;
	Schakel* begin;
	Schakel* einde;
};

SchakelLijst* maakSchakelLijst(size_t onderdeel_grootte);

void schakellijstVoeg(SchakelLijst* lijst, void* onderdeel);
// TODO void slijstVoegIn(SchakelLijst* lijst, unsigned int plek, void* onderdeel);

#define schakellijstKrijg(lijst, plek, soort) *((soort*)schakellijstKrijg_v(lijst, plek))

void schakellijstVerwijder(SchakelLijst* lijst, unsigned int plek);

void verwijderSchakelLijst(SchakelLijst* lijst);

// TODO voor elk & print

#endif