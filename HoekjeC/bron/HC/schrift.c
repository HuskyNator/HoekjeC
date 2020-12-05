#include "schrift.h"

#include "algemeen.h"

#include <stdlib.h>
#include <string.h>

const char nulteken = '\0';

/**		MAAK		**/

Schrift* maakSchrift(const char* inhoud) {
	if (inhoud == NULL) return maakLijst(10, sizeof(char));

	const size_t tel = strlen(inhoud);
	Schrift* schrift = maakLijst(tel, sizeof(char));
	schriftVoegNaMeer(schrift, inhoud, tel);
	return schrift;
}

Schrift* leesSchrift(FILE* bestand) {
	Schrift* schrift = maakSchrift(NULL);
	char c = getc(bestand);
	while (c != EOF) {
		lijstVoeg(schrift, &c);
		c = getc(bestand);
	}
	return schrift;
}

/**		GEBRUIK		**/

/*	Voegen	*/
inline void schriftVoegVoor(Schrift* schrift, char c) { lijstPlaats(schrift, 0, &c); }
inline void schriftVoegNa(Schrift* schrift, char c) { lijstVoeg(schrift, &c); }
inline void schriftVoegIn(Schrift* schrift, unsigned int plek, char c) { lijstPlaats(schrift, plek, &c); }

/*	Veelvoud Voegen	*/
inline void schriftVoegVoorMeer(Schrift* schrift, char* c, unsigned int lengte) {
	if (lengte == 0) lengte = strlen(c);
	lijstPlaatsMeer(schrift, 0, c, lengte);
}
inline void schriftVoegNaMeer(Schrift* schrift, char* c, unsigned int lengte) {
	if (lengte == 0) lengte = strlen(c);
	lijstVoegMeer(schrift, c, lengte);
}
inline void schriftVoegInMeer(Schrift* schrift, unsigned int plek, char* c, unsigned int lengte) {
	if (lengte == 0) lengte = strlen(c);
	lijstPlaatsMeer(schrift, plek, c, lengte);
}

/*	Krijgen	*/
inline char schriftKrijg(const Schrift* schrift, unsigned int plek) { return *lijstKrijg(schrift, plek, char); }

/*	Vervangen	*/
void schriftVervangChar(Schrift* schrift, char oud, char nieuw) {
	for (unsigned int i = 0; i < schrift->tel; i++) {
		if (*lijstKrijg(schrift, i, char) == oud) *lijstKrijg(schrift, i, char) = nieuw;
	}
}

char schriftVervang(Schrift* schrift, unsigned int plek, char nieuw) {
	if (plek >= schrift->tel) return 0;
	char* char_plek = lijstKrijg(schrift, plek, char);
	const char oud = *char_plek;
	*char_plek = nieuw;
	return oud;
}

void schriftVervangDeel(Schrift* schrift, unsigned int plek, char* nieuw, unsigned int lengte) {
	if (lengte == 0) lengte = (unsigned int)strlen(nieuw);
	if (plek > schrift->tel) return;  // Kan niet.
	const int eindverschil = lengte - schrift->tel + plek;
	const unsigned int navoegtal = eindverschil > 0 ? eindverschil : 0;
	const unsigned int eindpunt = plek + lengte - navoegtal;

	for (unsigned int i = plek; i < eindpunt; i++) {
		((char*)schrift->inhoud)[i] = nieuw[i];
	}
	if (navoegtal > 0) {
		char* navoegsel = nieuw + lengte - navoegtal;
		schriftVoegNaMeer(schrift, navoegsel, navoegtal);
	}
}

/* Verwijderen	*/
inline void schriftVerwijder(Schrift* schrift, unsigned int plek) { lijstVerwijder(schrift, plek, NULL); }

inline void schriftVerwijderReeks(Schrift* schrift, unsigned int van, unsigned int tot) {
	lijstVerwijderReeks(schrift, van, tot, NULL);
}

/*	Vergelijken	*/
booleaan schriftGelijk(const Schrift* schrift1, const Schrift* schrift2) {
	if (schrift1->tel != schrift2->tel) return onwaar;
	if (schrift1->inhoud == schrift2->inhoud) return waar;
	for (unsigned int i = 0; i < schrift1->tel; i++) {
		if (schriftKrijg(schrift1, i) != schriftKrijg(schrift2, i)) return onwaar;
	}
	return waar;
}
booleaan schriftDeelGelijk(const Schrift* schrift1, unsigned int plek, const Schrift* schrift2) {
	if (schrift1->tel - plek < schrift2->tel) return onwaar;
	for (unsigned int i = 0; i < schrift2->tel; i++) {
		if (*lijstKrijg(schrift1, plek + i, char) != *lijstKrijg(schrift2, i, char)) return onwaar;
	}
	return waar;
}
inline booleaan schriftVergelijker(const Schrift* schrift1, const Schrift* schrift2, size_t sleutel_grootte) {
	return schriftGelijk(schrift1, schrift2);
}

/*	Sleutelen	*/
// Bron: https://stackoverflow.com/a/7666668/12640952
unsigned int schriftSleutel(const Schrift* schrift) {
	unsigned int sleutel = 0x55555555;

	for (unsigned int i = 0; i < schrift->tel; i++) {
		sleutel ^= schriftKrijg(schrift, i);		// XOR
		sleutel = (sleutel << 5) | (sleutel >> 3);	// Schuif 5 naar links (met overloop)
	}

	return sleutel;
}

/*	Printen	*/
void schriftAfdrukken(const Schrift* schrift) {
	lijstLus(schrift, i, char) { putchar(*i); }
}

/**		VERWIJDER		**/

void verwijderSchrift(Schrift* schrift) { verwijderLijst(schrift, NULL); }

/**		VERANDER		**/

char* schriftNaarChar(Schrift* schrift) {
	lijstGroei(schrift, schrift->tel + 1);
	lijstVoeg(schrift, &nulteken);
	char* blok = schrift->inhoud;
	free(schrift);
	return blok;
}
