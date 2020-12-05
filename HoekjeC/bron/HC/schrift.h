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
void schriftVoegVoorMeer(Schrift* schrift, char* c, unsigned int lengte);
void schriftVoegNaMeer(Schrift* schrift, char* c, unsigned int lengte);
void schriftVoegInMeer(Schrift* schrift, unsigned int plek, char* c, unsigned int lengte);

/**
 * @brief  Voegt een veelvoud aan karakters in.
 * @param  plek: Waar het ingevoegd moet worden. (meetellend)
 * @param  nieuw: Wat ingevoegd moet worden.
 * @param  lengte: Hoe veel hiervan ingevoegd moet worden. Indien 0 wordt ingevoegd tot een '\\0' tegengekomen wordt.
 */
void schriftVoegMeer(Schrift* schrift, unsigned int plek, char* nieuw, unsigned int lengte);

/*	Krimpen	*/
#define schriftKrimp(schrift) lijstKrimp(schrift);

/*	Krijgen	*/
char schriftKrijg(const Schrift* schrift, unsigned int plek);

/*	Vervangen	*/
void schriftVervangChar(Schrift* schrift, char oud, char nieuw);
char schriftVervang(Schrift* schrift, unsigned int plek, char nieuw);  // Geeft vorige char terug, indien mogelijk.
void schriftVervangDeel(Schrift* schrift, unsigned int plek, char* nieuw, unsigned int lengte);

/* Verwijderen	*/
void schriftVerwijder(Schrift* schrift, unsigned int plek);								// Verwijdert char op plek, indien mogelijk.
void schriftVerwijderReeks(Schrift* schrift, unsigned int van, unsigned int tot);

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