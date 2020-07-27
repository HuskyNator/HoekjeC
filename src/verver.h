#ifndef VERVER_H
#define VERVER_H

#include "lineair.h"
#include "materiaal.h"

typedef struct Verver Verver;

struct Verver {
	unsigned int verfProgramma;
};

Verver* maakVerver(const char* hoekVerfLocatie, const char* scherfVerfLocatie);
void verwijderVerver(Verver* verver);

void gebruikVerver(const Verver* verver);
void zetVerverInt(const Verver* verver, const char* naam, int waarde);
void zetVerverFloat(const Verver* verver, const char* naam, float waarde);
void zetVerverFloat3v(const Verver* verver, const char* naam, float waarden[]);
void zetVerverFloat4v(const Verver* verver, const char* naam, float waarden[]);
void zetVerverMat4f(const Verver* verver, const char* naam, const Mat4f* mat);
void zetVerverMateriaal(const Verver* verver, const Materiaal* materiaal);
// En Meer

#endif