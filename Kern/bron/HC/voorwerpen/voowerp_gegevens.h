#ifndef VOORWERP_GEGEVENS_H
#define VOORWERP_GEGEVENS_H

#include "HC/lijsten/lijst.h"
#include "HC/verf/materiaal.h"
#include "HC/wiskunde/lineair.h"

/**
 * @file
 * @short Gegevens voor verschillende voorwerpssoorten.
 *
 * Voorwerpen hebben een gegevensveld om soorteigen gegevens toe te staan, welk hier beschreven worden.
 */

/**		Voorwerp		**/

/**		Vormen		**/

typedef enum vorm_soort VormSoort;
typedef struct vorm_gegevens VormGegevens;

enum vorm_soort { VORMSOORT_LIJN = 0, VORMSOORT_KLEUR = 1, VORMSOORT_AFBEELDING = 2, VORMSOORT_MATERIAAL = 3 };

struct vorm_gegevens {
	VormSoort soort;
	booleaan rand;
	const Vec4f* rand_kleur;
	// union {
	const Vec4f* kleur;
	// unsigned int afbeelding;
	// Materiaal* materiaal;
	// };
};

#endif