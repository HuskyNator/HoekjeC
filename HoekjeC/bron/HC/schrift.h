#ifndef SCHRIFT_C
#define SCHRIFT_C

#include "lijsten/lijst.h"

extern const char nulteken;

typedef Lijst Schrift;	// char[]

/**		MAAK		**/

Schrift* maakSchrift(const char* inhoud);
Schrift* leesSchrift(FILE* bestand);

#define maakSchrift_const(tekens) \
	(Schrift) { .grootte = sizeof(tekens), .inhoud = tekens, .onderdeel_grootte = sizeof(char), .tel = sizeof(tekens) - 1 }

/**		GEBRUIK		**/

/*	Voegen	*/
void schriftVoegVoor(Schrift* schrift, char c);
void schriftVoegNa(Schrift* schrift, char c);
void schriftVoegIn(Schrift* schrift, unsigned int plek, char c);

/*	Veelvoud Voegen	*/
void schriftVoegVoor_s(Schrift* schrift, char* c, unsigned int lengte);
void schriftVoegNa_s(Schrift* schrift, char* c, unsigned int lengte);
void schriftVoegIn_s(Schrift* schrift, unsigned int plek, char* c, unsigned int lengte);

/*	Krimpen	*/
#define schriftKrimp(schrift) lijstKrimp(schrift);

/*	Krijgen	*/
char schriftKrijg(const Schrift* schrift, unsigned int plek);

/*	Vervangen	*/
void schriftVervang(Schrift* schrift, char oud, char nieuw);
// TODO vervang op plek.
// TODO verwijder op plek.

/*	Vergelijken	*/
booleaan schriftGelijk(const Schrift* schrift1, const Schrift* schrift2);
booleaan schriftDeelGelijk(const Schrift* schrift1, unsigned int plek, const Schrift* schrift2);
booleaan schriftVergelijker(const Schrift* schrift1, const Schrift* schrift2, size_t sleutel_grootte);

/*	Sleutelen	*/
unsigned int schriftSleutel(const Schrift* schrift);

/*	Printen	*/
void schriftAfdrukken(const Schrift* schrift);

// TODO Lus

/**		VERWIJDER		**/

void verwijderSchrift(Schrift* schrift);

/**		VERANDER		**/

/**
 * @brief  Verandert het schrift van een getelde lijst naar een nultekenbeëindigd blok.
 */
char* schriftNaarChar(Schrift* schrift);

#endif