#ifndef LIJST_H
#define LIJST_H

#include "booleaan.h"

#include <stddef.h>

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

void lijstVoeg(Lijst* lijst, void* onderdeel);
booleaan lijstPlaats(Lijst* lijst, unsigned int plek, void* onderdeel);

/**
 * Alternatief kan ((onderdeel*)lijst->inhoud)[plek] gebruikt worden.
 */
void* lijstKrijg(Lijst* lijst, unsigned int plek);

booleaan lijstVerwijder(Lijst* lijst, unsigned int plek);
void verwijderLijst(Lijst* lijst);
/**
 * Zet de tel op nul.
 * Alternatief kan lijst->tel=0 gebruikt worden.
 */
void lijstLeeg(Lijst* lijst);

typedef void(*voor_elk_opdracht)(void* onderdeelverwijzing);
void lijstVoorElk(Lijst* lijst, voor_elk_opdracht opdracht);

// TODO ondersteun dmv vergelijkfuncties. int lijstVind(Lijst* lijst, void* onderdeel);
// TODO ondersteun dmv vergelijkfuncties. booleaan lijstVerwijderGelijk(Lijst* lijst, void* gelijk, booleaan bevrijd);
#endif