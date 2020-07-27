#ifndef OBJECTLEZER_H
#define OBJECTLEZER_H

#include "voorwerp.h"

typedef struct vlakGroep VlakGroep;
struct vlakGroep{
	Vlak* vlakken;
	unsigned int vlak_aantal;
};

char* leesBestand(const char* bestandsnaam);
Lijst* leesObj(const char* bestandsnaam);
void leesMtl(const char* bestandsnaam, Lijst* onderdelen);

#endif