#include "bestandslezer.h"

#include "lijst.h"
#include "lineair.h"
#include "vorm.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE* bestand;
booleaan EOF_gevonden, regeleind_gevonden;

Lijst* regel;	  // char*[]

Lijst* hoeken;	  // Vec3f*[]
Lijst* normalen;  // Vec3f*[]
// TODO Verf?

Lijst* hoektallen;	// Hoektal*[]

static char verwerpRuimte() {
	char teken = getc(bestand);
	while (teken == ' ' || teken == '\t' || teken == '\r' /*|| teken == '\n'*/) {
		teken = getc(bestand);
	}
	return teken;
}

static void verwerpRegel() {
	char teken;
	while (teken != '\n' && teken != EOF) {
		teken = getc(bestand);
	}
}

static char* leesWoord() {
	unsigned char woordTel = 0;
	unsigned char woordGrootte = 10;
	regeleind_gevonden = onwaar;
	char* woord = malloc(woordGrootte + 1);
	char teken = verwerpRuimte();
	while (teken != ' ' && teken != '\n' && teken != '\t' && teken != '\r' && teken != EOF) {
		if (teken == '#') {
			verwerpRegel();
			break;
		}
		if (woordTel == woordGrootte) {
			woordGrootte += 5;
			woord = realloc(woord, woordGrootte + 1);
		}
		woord[woordTel] = teken;
		woordTel++;
		teken = getc(bestand);
	}
	if (teken == EOF) EOF_gevonden = waar;
	else if (teken == '\n')
		regeleind_gevonden = waar;
	if (woordTel == 0) {
		free(woord);
		return NULL;
	}
	woord[woordTel] = '\0';
	return woord;
}

static void leesRegel() {
	lijstLeeg(regel);
	char* woord = leesWoord();
	while (woord != NULL) {
		lijstVoeg(regel, woord);
		if (EOF_gevonden || regeleind_gevonden) break;
		woord = leesWoord();
	}
}

static unsigned int vervang(char* woord, char aanwezig, char vervanging) {
	unsigned int tel;
	char teken;
	for (int i = 0; teken != '\0'; i++) {
		teken = woord[i];
		if (teken == aanwezig) {
			woord[i] = vervanging;
			tel++;
		}
	}
	return tel;
}

static void leesHoek() {
	leesRegel();
	if (regel->tel < 3 || regel->tel > 4) {
		fprintf(stderr, "Fout aantal woorden in obj hoek: %d", regel->tel);
		exit(-1);
	}
	float x = strtof(regel->inhoud[0], NULL);
	float y = strtof(regel->inhoud[1], NULL);
	float z = strtof(regel->inhoud[2], NULL);
	float w = 1;
	if (regel->tel == 4) w = strtof(regel->inhoud[3], NULL);
	Vec4f v = (Vec4f){x, y, z, w};
	Vec3f* v2 = malloc(sizeof(Vec3f));
	*v2 = Vec4n3f(v, waar);
	lijstVoeg(hoeken, v2);
}

static Vec3ui leesHoektal(char* hoektal) {
	Vec3ui h = {};
	char* getal_eind;
	vervang(hoektal, '\\', ' ');
	h.x = strtoul(hoektal, &getal_eind, 10);
	if (h.x != 0) {
		h.x -= 1;
		h.y = strtoul(getal_eind, &getal_eind, 10);
		if (h.y != 0) {
			h.y -= 1;
			h.z = strtoul(getal_eind, NULL, 10);
			if (h.z != 0) h.z -= 1;
		}
	}
	return h;
}

// TODO meer dan 3 hoeken
static void leesVlak() {
	leesRegel();
	if (regel->tel < 3) {
		fprintf(stderr, "Fout aantal hoeken in vlak: %d", regel->tel);
		exit(-1);
	}
	Hoektal* hoektal = malloc(sizeof(Hoektal));
	Vec3ui oorsprong = leesHoektal(regel->inhoud[0]);
	Vec3ui tweede = leesHoektal(regel->inhoud[1]);
	for (int i = 2; i < regel->tel; i++) {
		Vec3ui derde = leesHoektal(regel->inhoud[i]);
		hoektal->hoek = (Vec3ui){oorsprong.x, tweede.x, derde.x};
		hoektal->verf = (Vec3ui){oorsprong.y, tweede.y, derde.y};
		hoektal->normaal = (Vec3ui){oorsprong.z, tweede.z, derde.z};
		lijstVoeg(hoektallen, hoektal);
		tweede = derde;
	}
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

	regel = maakLijst(10);
	hoeken = maakLijst(10);
	hoektallen = maakLijst(10);

	while (!EOF_gevonden) {
		// Leidende Ruimte
		char* woord = leesWoord();
		if (woord == NULL) continue;

		// Aantekeningen
		if (strcmp(woord, "#") == 0) {
			verwerpRegel();
		} else if (strcmp(woord, "v") == 0) {
			leesHoek();
		} else if (strcmp(woord, "f") == 0) {
			leesVlak();
		}

		else {
			fprintf(stderr, "Onbekend begin van obj regel: %s\n", woord);
			if (!regeleind_gevonden) verwerpRegel();
		}
	}

	Vorm* vorm = maakVorm(hoeken, hoeken->tel * sizeof(Vec3f), hoektallen, hoektallen->tel * sizeof(Hoektal));
	fclose(bestand);
	verwijderLijst(regel, waar);
	verwijderLijst(hoeken, waar);
	verwijderLijst(normalen, waar);
	verwijderLijst(hoektallen, waar);
	return vorm;
}
