#include "bestandslezer.h"

#include "lineair.h"
#include "vorm.h"

#include <stdio.h>
#include <stdlib.h>

FILE* bestand;
char teken;
char* woord;
unsigned char woordGrootte, woordTel;
Vec3f* hoeken;
unsigned int hoekenGrootte, hoekenTel;
Vec3ui* hoektallen;
unsigned int hoektallenGrootte, hoektallenTel;

// Lees alle witruimte.
static void leesWitRuimte() {
	while (teken == ' ' || teken == '\t' || teken == '\r' || teken == '\n') {
		teken = getc(bestand);
	}
}

// Lees alle witruimte op \n na.
static void leesWitRegel() {
	while (teken == ' ' || teken == '\t') {
		teken = getc(bestand);
	}
}

static void leesRegel() {
	while (teken != '\n' && teken != EOF) {
		teken = getc(bestand);
	}
}

// Leest een woord beginnend met het teken.
// Woordtel is hierbij gelijk aan de uiteindelijke grootte.
static void leesWoord() {
	woordTel = 0;
	while (teken != ' ' && teken != '\n' && teken != '\t' && teken != EOF) {
		if (woordTel >= woordGrootte) {
			woordGrootte += 10;
			woord = realloc(woord, woordGrootte + 1);
		}
		woord[woordTel] = teken;
		woordTel++;
		teken = getc(bestand);
	}
	woord[woordTel] = '\0';
}

static void leesHoek() {
	teken = getc(bestand);
	leesWitRegel();
	leesWoord();
	float getal1 = strtof(woord, NULL);
	leesWitRegel();
	leesWoord();
	float getal2 = strtof(woord, NULL);
	leesWitRegel();
	leesWoord();
	float getal3 = strtof(woord, NULL);
	Vec3f v = (Vec3f){getal1, getal2, getal3};
	if (hoekenTel >= hoekenGrootte) {
		hoekenGrootte *= 2;
		hoeken = realloc(hoeken, hoekenGrootte * sizeof(Vec3f));
	}
	hoeken[hoekenTel] = v;
	hoekenTel++;
}

// TODO meer dan 3 hoeken
static void leesVlak() {
	teken = getc(bestand);

	unsigned char getaltel = 0;
	unsigned char getallengrootte = 3;
	unsigned int* getallen = malloc(getallengrootte * sizeof(unsigned int));
	while (1) {
		leesWitRegel();
		if (teken == '\n' || teken == EOF) break;
		if (getaltel >= getallengrootte) {
			getallengrootte *= 2;
			getallen = realloc(getallen, getallengrootte * sizeof(long));
		}

		leesWoord();
		unsigned int getal = strtol(woord, NULL, 10);

		if (getal < 0) {
			getallen[getaltel] = hoekenTel + getal;
		} else
			getallen[getaltel] = getal - 1;
		getaltel++;
	}

	if (getaltel != 3) {
		// TODO
		fprintf(stderr, "Vlak met meer of minder dan 3 hoeken: %d\n", getaltel);
	}

	Vec3ui v = (Vec3ui){getallen[0], getallen[1], getallen[2]};
	if (hoektallenTel == hoektallenGrootte) {
		hoektallenGrootte *= 2;
		realloc(hoektallen, hoektallenGrootte);
	}
	hoektallen[hoektallenTel] = v;
	hoektallenTel++;
}

char* leesBestand(const char* bestandsnaam) {
	FILE* bestand = fopen(bestandsnaam, "rb");
	if (bestand == NULL) {
		fprintf(stderr, "Bestand '%s' bestaat niet.", bestandsnaam);
		return NULL;
	}

	fseek(bestand, 0, SEEK_END);
	long grootte = ftell(bestand);
	fseek(bestand, 0, SEEK_SET);

	char* inhoud = malloc(grootte + 1);
	fread(inhoud, 1, grootte, bestand);
	*(inhoud + grootte) = '\0';
	fclose(bestand);
	return inhoud;
}

// TODO gebruik doorlees stukken voor te grote bestanden
Vorm* leesObj(const char* bestandsnaam) {
	bestand = fopen(bestandsnaam, "rb");
	if (bestand == NULL) {
		fprintf(stderr, "Bestand '%s' bestaat niet.", bestandsnaam);
		return NULL;
	}

	woordGrootte = 50;
	woord = malloc(woordGrootte + 1);

	hoekenGrootte = 8;
	hoeken = malloc(hoekenGrootte * sizeof(Vec3f));
	hoekenTel = 0;

	hoektallenGrootte = 8;
	hoektallen = malloc(hoektallenGrootte * sizeof(Vec3ui));
	hoektallenTel = 0;

	while (1) {
		// Leidende Ruimte
		teken = getc(bestand);
		leesWitRuimte();
		if (teken == EOF) break;

		// Aantekeningen
		if (teken == '#') {
			leesRegel();
		} else if (teken == 'v') {
			leesHoek();
		} else if (teken == 'f') {
			leesVlak();
		}

		else {
			fputs("Onbekend begin van obj regel: ", stderr);
			putc(teken, stderr);
			puts("");
			exit(-1);
		}

		if (teken == EOF) break;
		// else '\n'
	}

	Vorm* vorm = maakVorm(hoeken, hoekenTel * sizeof(Vec3f), hoektallen, hoektallenTel * sizeof(Vec3ui));
	fclose(bestand);
	free(woord);
	free(hoeken);
	free(hoektallen);
	return vorm;
}
