#ifndef LIJST_H
#define LIJST_H

#include "booleaan.h"

typedef struct lijst Lijst;
struct lijst {
	unsigned int grootte;
	unsigned int tel;
	void** inhoud;
};

Lijst* maakLijst(unsigned int grootte);
void lijstGroei(Lijst* lijst, unsigned int grootte);
void lijstKrimp(Lijst* lijst);

void lijstVoeg(Lijst* lijst, void* inhoud);
booleaan lijstPlaats(Lijst* lijst, unsigned int plek);

/**
 * Alternatief kan lijst->inhoud[plek] gebruikt worden.
 */
void* lijstKrijg(Lijst* lijst, unsigned int plek);
int lijstVind(Lijst* lijst, void* inhoud);

booleaan lijstVerwijder(Lijst* lijst, unsigned int plek, booleaan bevrijd);
booleaan lijstVerwijderGelijk(Lijst* lijst, void* gelijk, booleaan bevrijd);
void verwijderLijst(Lijst* lijst, booleaan bevrijd);
/**
 * Bevrijdt de inhoud & zet de tel op nul.
 * Alternatief kan lijst->tel=0 gebruikt worden.
 */
void lijstLeeg(Lijst* lijst);

#endif