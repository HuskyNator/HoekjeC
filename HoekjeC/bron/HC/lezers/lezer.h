#ifndef LEZER_H
#define LEZER_H

#include "lijsten/lijst.h"
#include "schrift.h"

#include <stdio.h>


Schrift* leesTot(FILE* bestand, char tot);

/**
 * @file
 * @short Leest delen van bestanden & geeft opdrachten om met woorden te werekn.
 */

char* leesRegel(FILE* bestand);

char* leesWoord(FILE* bestand);

booleaan woordBegintMet(char* woord, char* begin);

Lijst* woordSplits(char* woord, char teken);

#endif