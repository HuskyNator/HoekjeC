#include "materiaal.h"

void verwijderMateriaal(Materiaal* materiaal) {
	free(materiaal->naam);
	free(materiaal);
}
