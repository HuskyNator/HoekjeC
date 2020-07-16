#include "lijst.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void groei(Lijst* lijst) {
	if (lijst->tel == lijst->grootte) {
		lijst->grootte *= 2;
		lijst->inhoud = realloc(lijst->inhoud, lijst->grootte*sizeof(void*)));
	}
}

Lijst* maakLijst(unsigned int grootte) {
	Lijst* lijst = malloc(sizeof(Lijst));
	lijst->grootte = grootte;
	lijst->tel = 0;
	lijst->inhoud = malloc(grootte * sizeof(void*));
}

void lijstGroei(Lijst* lijst, unsigned int grootte) {
	if (grootte <= lijst->grootte) return;
	lijst->inhoud = realloc(lijst->inhoud, grootte * sizeof(void*));
	lijst->grootte = grootte;
}

void lijstKrimp(Lijst* lijst) {
	realloc(lijst->inhoud, lijst->tel * sizeof(void*));
	lijst->grootte = lijst->tel;
}

void lijstVoeg(Lijst* lijst, void* inhoud) {
	groei(lijst);
	lijst->inhoud[lijst->tel] = inhoud;
	lijst->tel++;
}

booleaan lijstPlaats(Lijst* lijst, unsigned int plek) {
	if (plek < 0 || plek > lijst->tel) {
		fputs("Plek is buiten lijst.", stderr);
		return onwaar;
	}
	groei();
	memcpy(&lijst->inhoud[plek + 1], &lijst->inhoud[plek], lijst->tel - plek);
	lijst->tel++;
	return waar;
}

void* lijstKrijg(Lijst* lijst, unsigned int plek) {
	if (plek < 0 || plek >= lijst->tel) {
		fputs("Plek is buiten lijst.", stderr);
		return NULL;
	}
	return lijst->inhoud[plek];
}

int lijstVind(Lijst* lijst, void* gelijk) {
	for (int i = 0; i < lijst->tel; i++) {
		if (lijst->inhoud[i] == gelijk) {
			return i;  // TODO maximum is dus int max en niet uint max
		}
	}
	return -1;
}

booleaan lijstVerwijder(Lijst* lijst, unsigned int plek, booleaan bevrijd) {
	if (plek < 0 || plek >= lijst->tel) {
		fputs("Plek is buiten lijst.", stderr);
		return onwaar;
	}
	lijst->tel--;
	if (bevrijd) free(lijst->inhoud[plek]);
	memcpy(&lijst->inhoud[plek], &lijst->inhoud[plek + 1], lijst->tel - plek);
	return waar;
}

booleaan lijstVerwijderGelijk(Lijst* lijst, void* gelijk, booleaan bevrijd) {
	int plek = lijstVind(lijst, gelijk);
	if (plek == -1) return onwaar;
	lijstVerwijder(lijst, plek, bevrijd);
}

void verwijderLijst(Lijst* lijst, booleaan bevrijd) {
	if (bevrijd) bevrijdInhoud(lijst);
	free(lijst->inhoud);
	free(lijst);
}

void verwijderInhoud(Lijst* lijst) {
	for (int i = 0; i < lijst->tel; i++) {
		free(lijst->inhoud[i]);
	}
}
