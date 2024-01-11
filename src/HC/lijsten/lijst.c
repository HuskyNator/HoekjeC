#include "lijst.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void groei(Lijst* lijst) {
	if (lijst->tel == lijst->grootte) {
		lijst->grootte *= 2;
		lijst->inhoud = realloc(lijst->inhoud, lijst->grootte * lijst->onderdeel_grootte);
	}
}

Lijst* maakLijst(unsigned int grootte, size_t onderdeel_grootte) {
	Lijst* lijst = malloc(sizeof(Lijst));
	lijst->grootte = grootte;
	lijst->tel = 0;
	lijst->onderdeel_grootte = onderdeel_grootte;
	lijst->inhoud = malloc(grootte * onderdeel_grootte);
	return lijst;
}

void lijstGroei(Lijst* lijst, unsigned int grootte) {
	if (grootte <= lijst->grootte) return;
	lijst->inhoud = realloc(lijst->inhoud, grootte * lijst->onderdeel_grootte);
	lijst->grootte = grootte;
}

void lijstKrimp(Lijst* lijst) {
	realloc(lijst->inhoud, lijst->tel * lijst->onderdeel_grootte);
	lijst->grootte = lijst->tel;
}

unsigned int lijstVoeg(Lijst* lijst, void* inhoud) {
	groei(lijst);
	memcpy(lijst->inhoud + lijst->tel * lijst->onderdeel_grootte, inhoud, lijst->onderdeel_grootte);
	lijst->tel++;
	return lijst->tel - 1;
}

booleaan lijstPlaats(Lijst* lijst, unsigned int plek, void* onderdeel) {
	if (plek < 0 || plek > lijst->tel) {
		fputs("Plek is buiten lijst.", stderr);
		return onwaar;
	}
	groei(lijst);
	memcpy(lijst->inhoud + (plek + 1) * lijst->onderdeel_grootte, lijst->inhoud + plek * lijst->onderdeel_grootte,
		   (lijst->tel - plek) * lijst->onderdeel_grootte);
	memcpy(lijst->inhoud + plek * lijst->onderdeel_grootte, onderdeel, lijst->onderdeel_grootte);
	lijst->tel++;
	return waar;
}

// void* lijstKrijg(Lijst* lijst, unsigned int plek) {
// 	if (plek < 0 || plek >= lijst->tel) {
// 		fputs("Plek is buiten lijst.", stderr);
// 		return NULL;
// 	}
// 	return lijst->inhoud + plek * lijst->onderdeel_grootte;
// }

booleaan lijstVind(Lijst* lijst, const void* onderdeel, unsigned int* plek) {
	for (unsigned int i = 0; i < lijst->tel; i++) {
		void* ander = lijst->inhoud + i * lijst->onderdeel_grootte;
		if (memcmp(onderdeel, ander, lijst->onderdeel_grootte) == 0) {
			if (plek != NULL) {
				*plek = i;
			}
			return waar;
		}
	}
	return onwaar;
}

booleaan lijstVerwijder(Lijst* lijst, unsigned int plek) {
	if (plek < 0 || plek >= lijst->tel) {
		fputs("Plek is buiten lijst.", stderr);
		return onwaar;
	}
	lijst->tel--;
	memcpy(lijst->inhoud + (plek + 1) * lijst->onderdeel_grootte, &lijst->inhoud + plek * lijst->onderdeel_grootte,
		   lijst->tel - plek);
	return waar;
}

// booleaan lijstVerwijderGelijk(Lijst* lijst, void* gelijk, booleaan bevrijd) {
// 	int plek = lijstVind(lijst, gelijk);
// 	if (plek == -1) return onwaar;
// 	lijstVerwijder(lijst, plek, bevrijd);
// }

void verwijderLijst(Lijst* lijst, booleaan bevrijd) {
	if (bevrijd) {
		for (int i = 0; i < lijst->tel; i++) {
			free(lijstKrijg(lijst, i, void*));
		}
	}
	free(lijst->inhoud);
	free(lijst);
}

void lijstLeeg(Lijst* lijst, booleaan bevrijd) {
	if (bevrijd)
		for (int i = 0; i < lijst->tel; i++) {
			free(lijstKrijg(lijst, i, void*));
		}
	lijst->tel = 0;
}

void lijstVoorElk(Lijst* lijst, voor_elk_opdracht opdracht) {
	for (int i = 0; i < lijst->tel; i++) {
		opdracht(lijst->inhoud + i * lijst->onderdeel_grootte);
	}
}
