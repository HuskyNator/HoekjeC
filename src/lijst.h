#ifndef LIJST_H
#define LIJST_H

#include "booleaan.h"

#include <stddef.h>
#include <stdio.h>

typedef struct lijst Lijst;
struct lijst {
	unsigned int grootte;
	unsigned int tel;
	size_t onderdeel_grootte;
	void* inhoud;
};

Lijst* maakLijst(unsigned int grootte, size_t onderdeel_grootte);
void lijstGroei(Lijst* lijst, unsigned int grootte);
void lijstKrimp(Lijst* lijst);

unsigned int lijstVoeg(Lijst* lijst, void* onderdeel);
booleaan lijstPlaats(Lijst* lijst, unsigned int plek, void* onderdeel);

#define lijstKrijg(lijst, plek, soort) ((soort*)lijst->inhoud)[plek]
booleaan lijstVind(Lijst* lijst, const void* onderdeel, unsigned int* plek);

booleaan lijstVerwijder(Lijst* lijst, unsigned int plek);
/**
 * Verwijdert de lijst & bevrijdt de verwijzing.
 * Indien 'bevrijd' waar is wordt de inhoudt gelezen als verwijzingen & bevrijd.
 */
void verwijderLijst(Lijst* lijst, booleaan bevrijd);
/**
 * Zet de tel op nul.
 * Alternatief kan lijst->tel=0 gebruikt worden.
 */
void lijstLeeg(Lijst* lijst, booleaan bevrijd);

typedef void (*voor_elk_opdracht)(void* onderdeelverwijzing);
void lijstVoorElk(Lijst* lijst, voor_elk_opdracht opdracht);

void lijstPrint(const Lijst* lijst, const FILE* doel);

// TODO ondersteun dmv vergelijkfuncties. booleaan lijstVerwijderGelijk(Lijst* lijst, void* gelijk, booleaan bevrijd);
#endif