#ifndef LIJST_H
#define LIJST_H

#include "HC/algemeen.h"

#include <stddef.h>
#include <stdio.h>

/**
 * @file
 * @short Een zelfgroeiende lijst.
 *
 * Houdt een geheugenblok bij dat verduppelt van grootte zodra deze vol is.
 * Er wordt gebruik maakt van verwijzingen om dingen over te schrijven naar de lijst,
 * om zo een algemene uitvoering mogelijk te kunnen maken.
 */

typedef struct lijst Lijst;
struct lijst {
	size_t onderdeel_grootte;
	unsigned int grootte;
	unsigned int tel;
	void* inhoud;
};

Lijst* maakLijst(unsigned int grootte, size_t onderdeel_grootte);
void lijstGroei(Lijst* lijst, unsigned int grootte);
void lijstKrimp(Lijst* lijst);

void lijstVoeg(Lijst* lijst, void* onderdeel);
void lijstVoegMeer(Lijst* lijst, void* onderdeel, unsigned int aantal);
booleaan lijstPlaats(Lijst* lijst, unsigned int plek, const void* onderdeel);
booleaan lijstPlaatsMeer(Lijst* lijst, unsigned int plek, const void* inhoud, unsigned int aantal);

#define lijstKrijg(lijst, plek, soort) ((soort*)(lijst->inhoud) + plek)
booleaan lijstVind(Lijst* lijst, void* onderdeel, vergelijk_opdracht vergelijker, unsigned int* plek);  // TODO vergelijkopdracht

booleaan lijstVerwijder(Lijst* lijst, unsigned int plek, verwijder_opdracht opdracht);
booleaan lijstVindVerwijder(Lijst* lijst, const void* onderdeel, vergelijk_opdracht vergelijker, verwijder_opdracht opdracht);
void lijstLeeg(Lijst* lijst, verwijder_opdracht opdracht);

/**
 * Verwijdert de lijst & bevrijdt de verwijzing.
 * Gebruikt de verwijder_opdracht op elk ingevoegd onderdeel.
 */
void verwijderLijst(Lijst* lijst, verwijder_opdracht opdracht);

// Het is aanradelijker een normale for loop te gebruiken.
#define voor_elk_lijst(lijst, i, soort) \
	unsigned int _i;                    \
	soort* i;                           \
	for (_i = 0, i = lijst->inhoud; _i < lijst->tel; _i = _i + 1, i = lijstKrijg(lijst, _i, soort))

void lijstPrint(const Lijst* lijst, const FILE* doel);

// TODO ondersteun dmv vergelijkfuncties. booleaan lijstVerwijderGelijk(Lijst* lijst, void* gelijk, booleaan bevrijd);
#endif