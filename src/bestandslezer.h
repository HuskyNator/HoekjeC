#ifndef OBJECTLEZER_H
#define OBJECTLEZER_H

#include "materiaal.h"
#include "voorwerp.h"

char* leesBestand(const char* bestandsnaam);
Vorm* leesObj(const char* bestandsnaam);
void leesMtl(const char* bestandsnaam, Lijst* materialen);

Materiaal* standaard_materiaal();

#endif