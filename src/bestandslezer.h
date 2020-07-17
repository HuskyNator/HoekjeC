#ifndef OBJECTLEZER_H
#define OBJECTLEZER_H

#include "vorm.h"

char* leesBestand(const char* bestandsnaam);
Vorm* leesObj(const char* bestandsnaam);
void /* ? */ leesMtl(const char* bestandsnaam);

#endif