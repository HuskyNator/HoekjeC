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

/**		MAAK		**/

Lijst* maakLijst(unsigned int grootte, size_t onderdeel_grootte);
// TODO LijstDeel

/**		GEBRUIK		**/

/*	Voegen & Plaatsen	*/
void lijstVoeg(Lijst* lijst, const void* onderdeel);
void lijstVoegMeer(Lijst* lijst, const void* onderdeel, unsigned int aantal);
booleaan lijstPlaats(Lijst* lijst, unsigned int plek, const void* onderdeel);
booleaan lijstPlaatsMeer(Lijst* lijst, unsigned int plek, const void* inhoud, unsigned int aantal);

// TODO vervang

/*	Krijgen & Vinden	*/
#define lijstKrijg(lijst, plek, soort) ((soort*)(lijst->inhoud) + plek)
booleaan lijstVind(const Lijst* lijst, const void* onderdeel, vergelijk_opdracht vergelijker,
				   unsigned int* plek);	 // TODO meerderen.

/*	Verwijderen & Legen	*/
booleaan lijstVerwijder(Lijst* lijst, unsigned int plek, verwijder_opdracht opdracht);
booleaan lijstVindVerwijder(Lijst* lijst, const void* onderdeel, vergelijk_opdracht vergelijker, verwijder_opdracht opdracht);
void lijstLeeg(Lijst* lijst, verwijder_opdracht opdracht);
// TODO lijstVerwijderMeer

/*	Grootte	*/
void lijstGroei(Lijst* lijst, unsigned int grootte);
void lijstKrimp(Lijst* lijst);

/*	Lus	*/
// Het is aanradelijker een normale for loop te gebruiken.
#define lijstLus(lijst, i, soort) \
	unsigned int _i;              \
	soort* i;                     \
	for (_i = 0, i = lijst->inhoud; _i < lijst->tel; _i = _i + 1, i = lijstKrijg(lijst, _i, soort))

/*	Afdrukken	*/
void lijstAfdrukken(Lijst* lijst, afdruk_opdracht opdracht);

/**		VERWIJDER		**/

/**
 * Verwijdert de lijst & bevrijdt de verwijzing.
 * Gebruikt de verwijder_opdracht op elk ingevoegd onderdeel.
 */
void verwijderLijst(Lijst* lijst, verwijder_opdracht opdracht);

// TODO ondersteun dmv vergelijkfuncties. booleaan lijstVerwijderGelijk(Lijst* lijst, void* gelijk, booleaan bevrijd);
#endif