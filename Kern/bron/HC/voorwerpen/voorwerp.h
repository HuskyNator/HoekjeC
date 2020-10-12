#ifndef VOORWERP2_H
#define VOORWERP2_H

#include "HC/booleaan.h"
#include "HC/verf/verver.h"
#include "HC/wiskunde/lineair.h"
#include "voowerp_gegevens.h"

typedef struct voorwerp Voorwerp;
struct voorwerp {
	const struct voorwerp_opdrachten* opdrachten;
	union {
		void* gegevens;
		VormGegevens* vormgegevens;
	};
	Vec3f plek;
	Vec3f grootte;
	Vec3f draaiing;
	Mat4f voorwerpM;
	booleaan bijgewerkt;
};

struct voorwerp_opdrachten {  // TODO: toch terugzetten in voorwerp?
	void (*teken_opdracht)(Voorwerp* voorwerp, Verver verver);
	void (*verwijder_opdracht)(Voorwerp* voorwerp);
};

void voorwerpZetPlek(Voorwerp* voorwerp, Vec3f plek);
void voorwerpZetGrootte(Voorwerp* voorwerp, Vec3f grootte);
void voorwerpZetDraai(Voorwerp* voorwerp, Vec3f draai);

void voorwerpZetM(Voorwerp* vorm);

void voorwerpTeken(Voorwerp* voorwerp, Verver verver);

void verwijderVoorwerp(Voorwerp* voorwerp);

#endif