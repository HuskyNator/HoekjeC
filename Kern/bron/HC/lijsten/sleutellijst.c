#include "sleutellijst.h"

#include <stdlib.h>
#include <string.h>

SleutelLijst* maakSleutelLijst(size_t sleutel_grootte, size_t waarde_grootte, unsigned int grootte,
							   sleutel_opdracht sleutelaar) {
	SleutelLijst* lijst = malloc(sizeof(SleutelLijst) + grootte * sizeof(SchakelLijst*));
	lijst->sleutel_grootte = sleutel_grootte;
	lijst->waarde_grootte = waarde_grootte;
	lijst->slot_grootte = sleutel_grootte + waarde_grootte;
	lijst->grootte = grootte;
	lijst->sleutelaar = sleutelaar;
	for (unsigned int i = 0; i < grootte; i++) {
		lijst->snippers[i] = maakSchakelLijst(lijst->slot_grootte);
	}
	return lijst;
}

void sleutellijstVoeg(SleutelLijst* lijst, void* sleutel, void* waarde) {
	int plek = lijst->sleutelaar(sleutel) % lijst->grootte;
	SchakelLijst* snipper = lijst->snippers[plek];
	char* slot = malloc(lijst->slot_grootte);
	memcpy(slot, sleutel, lijst->sleutel_grootte);
	memcpy(slot + lijst->sleutel_grootte, waarde, lijst->waarde_grootte);
	schakellijstVoeg(snipper, slot);
	free(slot);	 // TODO verbeter veel overschrijven
}

// TODO refactor.
booleaan sleutellijstVind(SleutelLijst* lijst, void* sleutel, void* gevonden_waarde) {
	int plek = lijst->sleutelaar(sleutel) % lijst->grootte;
	SchakelLijst* snipper = lijst->snippers[plek];
	Schakel* huidige = snipper->begin;
	for (unsigned int i = 0; i < snipper->tal; i++) {
		if (memcmp(huidige->inhoud, sleutel, lijst->sleutel_grootte) == 0) {
			memcpy(gevonden_waarde, huidige->inhoud + lijst->sleutel_grootte, lijst->waarde_grootte);
			return waar;
		}
		huidige = huidige->volgende;
	}
	return onwaar;
}

// TODO refactor.
booleaan sleutellijstVerwijder(SleutelLijst* lijst, void* sleutel) {
	int plek = lijst->sleutelaar(sleutel) % lijst->grootte;
	SchakelLijst* snipper = lijst->snippers[plek];
	Schakel* huidige = snipper->begin;
	for (unsigned int i = 0; i < snipper->tal; i++) {
		if (memcmp(huidige->inhoud, sleutel, lijst->sleutel_grootte) == 0) {
			schakellijstVerwijder(snipper, i);
			return waar;
		}
		huidige = huidige->volgende;
	}
	return onwaar;
}
#include <stdio.h>
void verwijderSleutelLijst(SleutelLijst* lijst) {
	for (unsigned int i = 0; i < lijst->grootte; i++) {
		verwijderSchakelLijst(lijst->snippers[i]);
	}
	free(lijst);
}
