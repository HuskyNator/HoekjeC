#ifndef LOGO_H
#define LOGO_H

#include "HC/kleuren.h"
#include "HC/verf/standaard_ververs.h"
#include "HC/voorwerpen/groep.h"
#include "HC/voorwerpen/voorwerp.h"
#include "HC/voorwerpen/vormen.h"
#include "HC/wiskunde/lineair.h"
#include "algemeen.h"

#include <GL/glew.h>
#include <stdlib.h>
#include <string.h>

/**
 * @file
 * @short Logo van HoekjeC
 *
 * Kan gebruikt worden om het logo te tekenen.
 *
 * @todo Gebruik het nieuwe voorwerpontwerp en maak plaatselijke tekening mogelijk.
 * @todo Verplaats groot deel naar .c bestand.
 */

void logo_teken_opdracht(Groep* groep, Verver verver);

extern const struct voorwerp_opdrachten logo_opdrachten;

// TODO: maak tot 'standaard' voorwerp, zodat je de hoeken kunt hergebruiken.
Groep* maakLogo(Vec3f plek, Vec3f grootte, Vec3f draaiing);

#endif