#ifndef VOORWERP_H
#define VOORWERP_H

#include "lineair.h"
#include "verver.h"
#include "vorm.h"

typedef struct voorwerp Voorwerp;

Voorwerp* maakVoorwerp(Vorm* vorm, Vec3f plaats, Vec3f grootte, Vec3f draai);

void tekenVoorwerp(Voorwerp* voorwerp, const Verver* verver);

#endif