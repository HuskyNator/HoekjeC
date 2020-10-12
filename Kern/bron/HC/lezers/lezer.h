#ifndef LEZER_H
#define LEZER_H

#include <stdio.h>
#include "lijsten/lijst.h"

char* leesRegel(FILE* bestand);

char* leesWoord(FILE* bestand);

booleaan woordBegintMet(char* woord, char* begin);

Lijst* woordSplits(char* woord, char teken);

#endif