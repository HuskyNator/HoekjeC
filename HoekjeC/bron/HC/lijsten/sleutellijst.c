#include "sleutellijst.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

SleutelLijst* maakSleutelLijst(size_t sleutel_grootte, size_t waarde_grootte, unsigned int emmer_aantal, sleutel_opdracht sleutelaar,
							   vergelijk_opdracht vergelijker) {
	SleutelLijst* lijst = malloc(sizeof(SleutelLijst) + emmer_aantal * sizeof(SchakelLijst*));
	lijst->sleutel_grootte = sleutel_grootte;
	lijst->waarde_grootte = waarde_grootte;
	lijst->emmer_aantal = emmer_aantal;
	lijst->tel = 0;
	lijst->sleutelaar = sleutelaar;
	lijst->vergelijker = (vergelijker == NULL) ? geheugen_vergelijker : vergelijker;
	for (unsigned int i = 0; i < emmer_aantal; i++) {
		lijst->emmers[i] = maakSchakelLijst(sizeof(Slot));
	}
	return lijst;
}

SleutelLijst* sleutellijstVerbeter(SleutelLijst* lijst) {
	unsigned int emmer_aantal = (float)lijst->tel / 0.75 + 1;
	SleutelLijst* nieuw =
		maakSleutelLijst(lijst->sleutel_grootte, lijst->waarde_grootte, emmer_aantal, lijst->sleutelaar, lijst->vergelijker);
	struct SleutelLijst_lusser _lusser;
	Slot* i;
	for (_lusser = sleutellijst_maakLusser(lijst), i = (Slot*)_lusser.schakel->inhoud; i != NULL;
		 sleutellijst_lusserVolgende(&_lusser), i = _lusser.schakel == NULL ? NULL : (Slot*)_lusser.schakel->inhoud) {
		sleutellijstVoeg(nieuw, i->sleutel, i->waarde);
	}
	verwijderSleutelLijst(lijst, NULL, NULL);
	return nieuw;
}

// TODO: wat als de sleutel het zelfde is?
void sleutellijstVoeg(SleutelLijst* lijst, const void* sleutel, const void* waarde) {
	unsigned int plek = lijst->sleutelaar(sleutel) % lijst->emmer_aantal;
	SchakelLijst* emmer = lijst->emmers[plek];

	Slot slot = {};
	slot.sleutel = malloc(lijst->sleutel_grootte);
	slot.waarde = malloc(lijst->waarde_grootte);
	memcpy(slot.sleutel, sleutel, lijst->sleutel_grootte);
	memcpy(slot.waarde, waarde, lijst->waarde_grootte);

	schakellijstVoeg(emmer, &slot);
	lijst->tel++;
}

booleaan sleutellijstVind(SleutelLijst* lijst, const void* sleutel, void* gevonden_waarde) {
	int plek = lijst->sleutelaar(sleutel) % lijst->emmer_aantal;
	SchakelLijst* emmer = lijst->emmers[plek];
	schakellijstLus(emmer, i, Slot) {
		if (lijst->vergelijker(i->sleutel, sleutel, lijst->sleutel_grootte)) {
			if (gevonden_waarde != NULL) memcpy(gevonden_waarde, i->waarde, lijst->waarde_grootte);
			return waar;
		}
	}
	return onwaar;
}

booleaan sleutellijstVerwijder(SleutelLijst* lijst, const void* sleutel, verwijder_opdracht sleutel_opdracht,
							   verwijder_opdracht waarde_opdracht) {
	unsigned int plek = lijst->sleutelaar(sleutel) % lijst->emmer_aantal;
	SchakelLijst* emmer = lijst->emmers[plek];

	unsigned int i_plek = 0;
	schakellijstLus(emmer, i, Slot) {
		if (lijst->vergelijker(i->sleutel, sleutel, lijst->sleutel_grootte)) {
			if (sleutel_opdracht != NULL) sleutel_opdracht(i->sleutel);
			if (waarde_opdracht != NULL) waarde_opdracht(i->waarde);
			schakellijstVerwijder(emmer, i_plek, NULL);
			lijst->tel--;
			return waar;
		}
		i_plek++;
	}
	return onwaar;
}

/*	Lus	*/

struct SleutelLijst_lusser sleutellijst_maakLusser(SleutelLijst* lijst) {
	struct SleutelLijst_lusser lusser = {.lijst = lijst, .schakel = NULL, .emmer = 0};
	if (lijst->tel == 0) return lusser;
	for (unsigned int i = 0; i < lijst->emmer_aantal; i++) {
		SchakelLijst* emmer = lijst->emmers[i];
		if (emmer->begin != NULL) {
			lusser.emmer = i;
			lusser.schakel = emmer->begin;
			return lusser;
		}
	}
	fputs("Sleutellijst_lusser wordt gemaakt maar kon geen schakel vinden, terwijl de lijst niet leeg is!", stderr);
	exit(1);
}

void sleutellijst_lusserVolgende(struct SleutelLijst_lusser* lusser) {
	if (lusser->schakel != NULL) lusser->schakel = lusser->schakel->volgende;
	if (lusser->schakel != NULL) return;  // Schakel gevonden.

	const SleutelLijst* lijst = lusser->lijst;
	const unsigned int emmer_aantal = lijst->emmer_aantal;
	for (unsigned int i = lusser->emmer + 1; i < emmer_aantal; i++) {
		SchakelLijst* emmer = lijst->emmers[i];
		if (emmer->tel == 0) continue;
		lusser->emmer = i;
		lusser->schakel = emmer->begin;
		return;	 // Schakel gevonden.
	}
	return;	 // Kan niet meer vinden.
}

/*	Afdrukken	*/
void sleutellijstAfdrukken(SleutelLijst* lijst, afdruk_opdracht sleutel_opdracht, afdruk_opdracht waarde_opdracht) {
	putchar('{');
	const unsigned int aantal = lijst->tel;
	unsigned int tel = 1;
	sleutellijstLus(lijst, i) {
		sleutel_opdracht(i->sleutel);
		putchar(':');
		waarde_opdracht(i->waarde);
		if (aantal != tel) {
			putchar(' ');
			putchar(',');
		}
		tel++;
	}
	putchar('}');
}

void verwijderSleutelLijst(SleutelLijst* lijst, verwijder_opdracht sleutel_opdracht, verwijder_opdracht waarde_opdracht) {
	for (unsigned int i = 0; i < lijst->emmer_aantal; i++) {
		SchakelLijst* emmer = lijst->emmers[i];
		if (sleutel_opdracht != NULL || waarde_opdracht != NULL) {
			schakellijstLus(emmer, j, Slot) {
				if (sleutel_opdracht != NULL) sleutel_opdracht(j->sleutel);
				if (waarde_opdracht != NULL) waarde_opdracht(j->waarde);
			}
		}
		verwijderSchakelLijst(emmer, NULL);
	}
	free(lijst);
}
