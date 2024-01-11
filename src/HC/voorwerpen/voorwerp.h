#ifndef VOORWERP_H
#define VOORWERP_H

#include "HC/wiskunde/lineair.h"
#include "HC/verf/verver.h"
#include "vorm.h"

typedef struct voorwerp Voorwerp;

Voorwerp* maakVoorwerp(Vorm* vorm, Vec3f plaats, Vec3f grootte, Vec3f draai);

void tekenVoorwerp(Voorwerp* voorwerp, Verver* verver);

#endif