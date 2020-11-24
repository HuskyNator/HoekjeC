#include "schrift.h"

#include "algemeen.h"
#include "stdlib.h"

const char nulteken = '\0';

/**		MAAK		**/

Schrift* maakSchrift(const char* inhoud) {
	if (inhoud == NULL) return maakLijst(10, sizeof(char));

	char* teller = inhoud;
	while (*teller != '\0') {
		teller++;
	}

	const size_t tel = teller - inhoud;
	Schrift* schrift = maakLijst(tel, sizeof(char));
	schriftVoegNa_s(schrift, inhoud, tel);
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
inline void schriftVoegVoor_s(Schrift* schrift, char* c, unsigned int lengte) { lijstPlaatsMeer(schrift, 0, c, lengte); }
inline void schriftVoegNa_s(Schrift* schrift, char* c, unsigned int lengte) { lijstVoegMeer(schrift, c, lengte); }
inline void schriftVoegIn_s(Schrift* schrift, unsigned int plek, char* c, unsigned int lengte) {
	lijstPlaatsMeer(schrift, plek, c, lengte);
}

/*	Krijgen	*/
inline char schriftKrijg(const Schrift* schrift, unsigned int plek) { return *lijstKrijg(schrift, plek, char); }

/*	Vervangen	*/
void schriftVervang(Schrift* schrift, char oud, char nieuw) {
	for (unsigned int i = 0; i < schrift->tel; i++) {
		if (*lijstKrijg(schrift, i, char) == oud) *lijstKrijg(schrift, i, char) = nieuw;
	}
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
