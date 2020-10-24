#ifndef VOORWERP_GEGEVENS_H
#define VOORWERP_GEGEVENS_H

#include "HC/wiskunde/lineair.h"
#include "HC/verf/materiaal.h"

/**		Voorwerp		**/

/**		Vormen		**/

typedef enum vorm_soort VormSoort;
typedef struct vorm_gegevens VormGegevens;

enum vorm_soort { VORMSOORT_KLEUR = 0, VORMSOORT_AFBEELDING = 1, VORMSOORT_MATERIAAL = 2 };

struct vorm_gegevens {
	VormSoort soort;
	union {
		Vec4f* kleur;
		unsigned int afbeelding;
		Materiaal* materiaal;
	};
};

#endif