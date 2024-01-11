#ifndef SNIPPERLIJST_H
#define SNIPPERLIJST_H

#include "schakellijst.h"

typedef unsigned int (*sleutel_opdracht)(const void* sleutel);

typedef struct sleutellijst SleutelLijst;
struct sleutellijst {
	size_t sleutel_grootte;
	size_t waarde_grootte;
	size_t slot_grootte;
	unsigned int grootte;
	unsigned int tal;
	sleutel_opdracht sleutelaar;
	SchakelLijst* snippers[];
};

SleutelLijst* maakSleutelLijst(size_t sleutel_grootte, size_t waarde_grootte, unsigned int grootte, sleutel_opdracht sleutelaar);

void sleutellijstVoeg(SleutelLijst* lijst, void* sleutel, void* waarde);

booleaan sleutellijstVind(SleutelLijst* lijst, void* sleutel, void* gevonden_waarde);

booleaan sleutellijstVerwijder(SleutelLijst* lijst, void* sleutel);

void verwijderSleutelLijst(SleutelLijst* lijst);

#endif