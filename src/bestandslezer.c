#include "bestandslezer.h"

#include "lineair.h"
#include "vorm.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE* bestand;
booleaan EOF_gevonden, regeleind_gevonden;
char** regel;
unsigned int regelGrootte, regelTel;

Vec3f *hoeken, *normalen;
unsigned int hoekenGrootte, hoekenTel, normalenGrootte, normalenTel;

Vec3ui *hoektallen, *hoekVerfTallen, *hoekNormaalTallen;
unsigned int hoektallenGrootte, hoektallenTel;

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
	regelTel = 0;
	char* woord = leesWoord();
	while (woord != NULL) {
		if (regelTel == regelGrootte) {
			regelGrootte += 5;
			regel = realloc(regel, regelGrootte * sizeof(char*));
			memset(regel + regelTel, 0, (regelGrootte - regelTel) * sizeof(char*));
		}
		free(regel[regelTel]);
		regel[regelTel] = woord;
		regelTel++;
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
	if (regelTel < 3 || regelTel > 4) {
		fprintf(stderr, "Fout aantal woorden in obj hoek: %d", regelTel);
		exit(-1);
	}
	float x = strtof(regel[0], NULL);
	float y = strtof(regel[1], NULL);
	float z = strtof(regel[2], NULL);
	float w = 1;
	if (regelTel == 4) w = strtof(regel[3], NULL);
	Vec4f v = (Vec4f){x, y, z, w};
	Vec3f v2 = Vec4n3f(v, waar);
	if (hoekenTel == hoekenGrootte) {
		hoekenGrootte *= 2;
		hoeken = realloc(hoeken, hoekenGrootte * sizeof(Vec3f));
	}
	hoeken[hoekenTel] = v2;
	hoekenTel++;
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
	if (regelTel < 3) {
		fprintf(stderr, "Fout aantal hoeken in vlak: %d", regelTel);
		exit(-1);
	}
	Vec3ui oorsprong = leesHoektal(regel[0]);
	Vec3ui tweede = leesHoektal(regel[1]);
	for (int i = 2; i < regelTel; i++) {
		Vec3ui derde = leesHoektal(regel[i]);
		if (hoektallenTel == hoektallenGrootte) {
			hoektallenGrootte *= 2;
			hoektallen = realloc(hoektallen, hoektallenGrootte * sizeof(Vec3ui));
			hoekVerfTallen = realloc(hoekVerfTallen, hoektallenGrootte * sizeof(Vec3ui));
			hoekNormaalTallen = realloc(hoekNormaalTallen, hoektallenGrootte * sizeof(Vec3ui));
		}
		hoektallen[hoektallenTel] = (Vec3ui){oorsprong.x, tweede.x, derde.x};
		hoekVerfTallen[hoektallenTel] = (Vec3ui){oorsprong.y, tweede.y, derde.y};
		hoekNormaalTallen[hoektallenTel] = (Vec3ui){oorsprong.z, tweede.z, derde.z};
		hoektallenTel++;
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

	hoekenGrootte = 8;
	hoekenTel = 0;
	hoeken = malloc(hoekenGrootte * sizeof(Vec3f));

	hoektallenGrootte = 8;
	hoektallenTel = 0;
	hoektallen = malloc(hoektallenGrootte * sizeof(Vec3ui));
	hoekVerfTallen = malloc(hoektallenGrootte * sizeof(Vec3ui));
	hoekNormaalTallen = malloc(hoektallenGrootte * sizeof(Vec3ui));

	regelGrootte = 5;
	regelTel = 0;
	regel = calloc(regelGrootte, sizeof(char*));

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
			if(!regeleind_gevonden) verwerpRegel();
		}
	}

	Vorm* vorm = maakVorm(hoeken, hoekenTel * sizeof(Vec3f), hoektallen, hoektallenTel * sizeof(Vec3ui));
	fclose(bestand);
	free(hoeken);
	free(hoektallen);
	free(hoekVerfTallen);
	free(hoekNormaalTallen);
	for (int i = 0; i < regelGrootte; i++) free(regel[i]);
	free(regel);
	return vorm;
}
