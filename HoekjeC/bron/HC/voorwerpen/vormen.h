#ifndef VORMEN_H
#define VORMEN_H

#include "voorwerp.h"

/**
 * @file
 * @short Simpele voorwerpen zoals vlakken en blokken.
 */

typedef Voorwerp Vorm;

void vormZetKleur(Voorwerp* voorwerp, const Vec4f* kleur);

/**		Driekant		**/
typedef Vorm Driekant;
Driekant* maakKant();

/**		Vlak		**/
typedef Vorm Vierkant;
Vierkant* maakVierkant();

/**		Blok		**/
typedef Vorm Blok;
Blok* maakBlok();

/**		Driehoek		**/
typedef Vorm Driehoek;
Driehoek* maakDriehoek(const Vec3f* plekken, const Vec3f* normalen, const Vec2f* verfplekken);

/**		Vierhoek		**/
typedef Vorm Vierhoek;
Vierhoek* maakVierhoek(const Vec3f* plekken, const Vec3f* normalen, const Vec2f* verfplekken);

#endif