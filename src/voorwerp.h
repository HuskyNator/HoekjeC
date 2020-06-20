#ifndef VOORWERP_H
#define VOORWERP_H

#include "lineair.h"
#include "verver.h"
#include "vorm.h"

struct voorwerp {
	Vorm* vorm;
	Vec3f plaats;
	Vec3f grootte;
	float draaix;
	float draaiy;
	float draaiz;
	Mat4f voorwerpMatrix;
};

typedef struct voorwerp Voorwerp;

Voorwerp maakVoorwerp(Vorm* vorm, Vec3f plaats, Vec3f grootte, float draaix, float draaiy, float draaiz);

void tekenVoorwerp(const Voorwerp* voorwerp, const Verver* verver);

#endif