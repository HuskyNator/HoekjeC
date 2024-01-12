#ifndef OBJECTLEZER_H
#define OBJECTLEZER_H

#include "HC/verf/materiaal.h"
#include "HC/voorwerpen/voorwerp.h"

char* leesBestand(const char* bestandsnaam);

// Allocates output string & concatinates 2 pre-existing source strings.
char* strConcat(const char* first, const char* second);
char* findDir(const char* filePath);
Vorm* leesObj(const char* bestandsnaam);
void leesMtl(const char* bestandsnaam, Lijst* materialen);

Materiaal* standaard_materiaal();

#endif