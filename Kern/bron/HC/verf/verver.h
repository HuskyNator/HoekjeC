#ifndef VERVER_H
#define VERVER_H

#include "HC/wiskunde/lineair.h"
#include "materiaal.h"

typedef unsigned int Verver;

Verver maakVerver(const char* hoekVerfLocatie, const char* scherfVerfLocatie);
void verwijderVerver(Verver verver);

// Stelt deze verver in voor gebruik indien deze dit nog niet was, en werkt zicht waardes bij.
void gebruikVerver(Verver verver);
void zetVerverInt(Verver verver, const char* naam, int waarde);

void zetVerverFloat(Verver verver, const char* naam, float waarde);
void zetVerverFloat3v(Verver verver, const char* naam, float waarden[]);
void zetVerverFloat4v(Verver verver, const char* naam, float waarden[]);

void zetVerverDouble(Verver verver, const char* naam, double waarde);
void zetVerverDouble3v(Verver verver, const char* naam, double waarden[]);
void zetVerverDouble4v(Verver verver, const char* naam, double waarden[]);

void zetVerverMat4f(Verver verver, const char* naam, const Mat4f* mat);
void zetVerverMateriaal(Verver verver, Materiaal* materiaal);
// En Meer

#endif