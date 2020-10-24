#ifndef VORMEN_H
#define VORMEN_H

#include "voorwerp.h"

typedef Voorwerp Vorm;

void vormZetKleur(Voorwerp* voorwerp, const Vec4f* kleur);

/**		Driehoek		**/
typedef Vorm Driehoek;
// TODO

/**		Vlak		**/
typedef Vorm Vlak;

Vlak* maakVlak();

/**		Blok		**/
typedef Vorm Blok;

Blok* maakBlok();
// TODO

#endif