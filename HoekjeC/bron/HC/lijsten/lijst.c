#include "lijst.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

inline static void* lijst_krijg(const Lijst* lijst, unsigned int plek) { return lijst->inhoud + lijst->onderdeel_grootte * plek; }

static void groei(Lijst* lijst) {
	if (lijst->tel == lijst->grootte) {
		lijst->grootte *= 2;
		lijst->inhoud = realloc(lijst->inhoud, lijst->grootte * lijst->onderdeel_grootte);
	}
}

Lijst* maakLijst(unsigned int grootte, size_t onderdeel_grootte) {
	Lijst* lijst = malloc(sizeof(Lijst));
	lijst->onderdeel_grootte = onderdeel_grootte;
	lijst->grootte = grootte;
	lijst->tel = 0;
	lijst->inhoud = malloc(grootte * onderdeel_grootte);
	return lijst;
}

void lijstGroei(Lijst* lijst, unsigned int grootte) {
	if (grootte <= lijst->grootte) return;
	lijst->inhoud = realloc(lijst->inhoud, grootte * lijst->onderdeel_grootte);
	lijst->grootte = grootte;
}

void lijstKrimp(Lijst* lijst) {
	lijst->inhoud = realloc(lijst->inhoud, lijst->tel * lijst->onderdeel_grootte);
	lijst->grootte = lijst->tel;
}

void lijstVoeg(Lijst* lijst, const void* inhoud) {
	groei(lijst);
	memcpy(lijst->inhoud + lijst->tel * lijst->onderdeel_grootte, inhoud, lijst->onderdeel_grootte);
	lijst->tel++;
}

void lijstVoegMeer(Lijst* lijst, const void* inhoud, unsigned int aantal) {
	lijstGroei(lijst, lijst->tel + aantal);
	memcpy(lijst->inhoud + lijst->tel, inhoud, aantal * lijst->onderdeel_grootte);
}

booleaan lijstPlaats(Lijst* lijst, unsigned int plek, const void* onderdeel) {
	if (plek >= lijst->tel) {
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

booleaan lijstPlaatsMeer(Lijst* lijst, unsigned int plek, const void* inhoud, unsigned int aantal) {
	if (plek >= lijst->tel) {
		fputs("Plek is buiten lijst.", stderr);
		return onwaar;
	}
	lijstGroei(lijst, lijst->tel + aantal);
	memcpy(lijst_krijg(lijst, plek + aantal), lijst_krijg(lijst, plek), (lijst->tel - plek) * lijst->onderdeel_grootte);
	memcpy(lijst_krijg(lijst, plek), inhoud, aantal * lijst->onderdeel_grootte);
}

booleaan lijstVind(const Lijst* lijst, const void* onderdeel, vergelijk_opdracht vergelijker, unsigned int* plek) {
	for (unsigned int i = 0; i < lijst->tel; i++) {
		void* ander = lijst_krijg(lijst, i);
		if (vergelijker(ander, onderdeel, lijst->onderdeel_grootte)) {
			if (plek != NULL) *plek = i;
			return waar;
		}
	}
	return onwaar;
}

booleaan lijstVerwijder(Lijst* lijst, unsigned int plek, verwijder_opdracht opdracht) {
	if (plek < 0 || plek >= lijst->tel) {
		fputs("Plek is buiten lijst.", stderr);
		return onwaar;
	}
	void* onderdeel = lijst->inhoud + lijst->onderdeel_grootte * plek;
	if (opdracht != NULL) opdracht(onderdeel);

	memcpy(lijst->inhoud + (plek + 1) * lijst->onderdeel_grootte, &lijst->inhoud + plek * lijst->onderdeel_grootte, lijst->tel - plek);
	lijst->tel--;
	return waar;
}

booleaan lijstVindVerwijder(Lijst* lijst, const void* onderdeel, vergelijk_opdracht vergelijker, verwijder_opdracht opdracht) {
	unsigned int plek;
	if (!lijstVind(lijst, onderdeel, vergelijker, &plek)) return onwaar;
	lijstVerwijder(lijst, plek, opdracht);
	return waar;
}

void lijstLeeg(Lijst* lijst, verwijder_opdracht opdracht) {
	if (opdracht != NULL) {
		for (int i = 0; i < lijst->tel; i++) {
			opdracht(lijst_krijg(lijst, i));
		}
	}
	lijst->tel = 0;
}

void verwijderLijst(Lijst* lijst, verwijder_opdracht opdracht) {
	if (lijst == NULL) return;
	if (opdracht != NULL) {
		for (int i = 0; i < lijst->tel; i++) {
			opdracht(lijst_krijg(lijst, i));
		}
	}
	free(lijst->inhoud);
	free(lijst);
}
