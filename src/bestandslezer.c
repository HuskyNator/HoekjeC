#include "bestandslezer.h"

#include "lijst.h"
#include "lineair.h"
#include "vorm.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE* bestand;
booleaan EOF_gevonden, regeleind_gevonden;

Lijst* regel;  // char*[]

Lijst* gegeven_hoeken_p;  // Vec3f[]
Lijst* gegeven_hoeken_v;  // Vec2f[]
Lijst* gegeven_hoeken_n;  // Vec3f[]

Lijst* hoeken_p;	  // Vec3f[]
Lijst* hoeken_v;	  // Vec2f[]
Lijst* hoeken_n;	  // Vec3f[]
Lijst* hoekentallen;  // Hoektallen[]
Lijst* vlakken;		  // Vlak[]

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
	lijstLeeg(regel, waar);
	char* woord = leesWoord();
	while (woord != NULL) {
		lijstVoeg(regel, &woord);
		if (EOF_gevonden || regeleind_gevonden) break;
		woord = leesWoord();
	}
}

static unsigned int vervang(char* woord, char aanwezig, char vervanging) {
	unsigned int tel;
	char teken = ' ';
	for (int i = 0; teken != '\0'; i++) {
		teken = woord[i];
		if (teken == aanwezig) {
			woord[i] = vervanging;
			tel++;
		}
	}
	return tel;
}

static void leesHoekP() {
	leesRegel();
	if (regel->tel < 3 || regel->tel > 4) {
		fprintf(stderr, "Verwachtte 3/4 woorden in .obj hoek(p) regel maar kreeg: %d\n", regel->tel);
		exit(-1);
	}
	float x = strtof(lijstKrijg(regel, 0, char*), NULL);
	float y = strtof(lijstKrijg(regel, 1, char*), NULL);
	float z = strtof(lijstKrijg(regel, 2, char*), NULL);
	float w = 1;
	if (regel->tel == 4) w = strtof(lijstKrijg(regel, 3, char*), NULL);
	Vec4f v = (Vec4f){x, y, z, w};
	Vec3f v2 = Vec4n3f(v, onwaar);
	lijstVoeg(gegeven_hoeken_p, &v2);
}

static void leesHoekN() {
	leesRegel();
	if (regel->tel != 3) {
		fprintf(stderr, "Verwachtte 3 woorden in .obj hoek(n) regel maar kreeg: %d\n", regel->tel);
		exit(-1);
	}
	float x = strtof(lijstKrijg(regel, 0, char*), NULL);
	float y = strtof(lijstKrijg(regel, 1, char*), NULL);
	float z = strtof(lijstKrijg(regel, 2, char*), NULL);
	Vec3f v = Vec3fn((Vec3f){x, y, z});
	lijstVoeg(gegeven_hoeken_n, &v);
}

static void leesHoekV() {
	leesRegel();
	if (regel->tel != 2) {
		fprintf(stderr, "Verwachtte 2 woorden in .obj hoek(v) regel maar kreeg: %d\n", regel->tel);
		exit(-1);
	}
	float x = strtof(lijstKrijg(regel, 0, char*), NULL);
	float y = strtof(lijstKrijg(regel, 1, char*), NULL);
	Vec2f v = (Vec2f){x, y};
	lijstVoeg(gegeven_hoeken_v, &v);
}

static Hoektallen leesHoektallen(char* hoektal) {
	Hoektallen h = {0, 0, 0};
	char* getal_eind;
	booleaan dubbel = strstr(hoektal, "//") != NULL;
	vervang(hoektal, '/', ' ');
	const float p = strtoul(hoektal, &getal_eind, 10);
	if (p == 0) return h;
	else if (p < 0)
		h.plek = gegeven_hoeken_p->tel + p;
	else
		h.plek = p - 1;

	if (!dubbel) {
		const float v = strtoul(getal_eind, &getal_eind, 10);
		if (v == 0) return h;
		else if (v < 0)
			h.verf = gegeven_hoeken_v->tel + v;
		else
			h.verf = v - 1;
	}

	const float n = strtoul(getal_eind, NULL, 10);
	if (n == 0) return h;
	else if (n < 0)
		h.normaal = gegeven_hoeken_n->tel + n;
	else
		h.normaal = n - 1;
	return h;
}

static unsigned int voegHoekToe(Hoektallen* hoektal) {
	unsigned int oude;
	if (lijstVind(hoekentallen, hoektal, &oude)) return oude;
	Vec3f p = lijstKrijg(gegeven_hoeken_p, hoektal->plek, Vec3f);
	Vec2f v = lijstKrijg(gegeven_hoeken_v, hoektal->verf, Vec2f);
	Vec3f n = lijstKrijg(gegeven_hoeken_n, hoektal->normaal, Vec3f);
	lijstVoeg(hoeken_p, &p);
	lijstVoeg(hoeken_v, &v);
	lijstVoeg(hoeken_n, &n);
	lijstVoeg(hoekentallen, hoektal);
	return hoekentallen->tel - 1;
}

static void leesVlak() {
	leesRegel();
	if (regel->tel < 3) {
		fprintf(stderr, "Fout aantal hoeken in vlak: %d", regel->tel);
		exit(-1);
	}

	Hoektallen oorsprong = leesHoektallen(lijstKrijg(regel, 0, char*));
	unsigned int oorsprongtal = voegHoekToe(&oorsprong);

	Hoektallen tweede = leesHoektallen(lijstKrijg(regel, 1, char*));
	unsigned int tweedetal = voegHoekToe(&tweede);

	for (int i = 2; i < regel->tel; i++) {
		Hoektallen derde = leesHoektallen(lijstKrijg(regel, i, char*));
		unsigned int derdetal = voegHoekToe(&derde);

		Vlak vlak = (Vlak){oorsprongtal, tweedetal, derdetal};
		lijstVoeg(vlakken, &vlak);
		// tweede = derde;
		tweedetal = derdetal;
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

	EOF_gevonden = onwaar;
	regeleind_gevonden = onwaar;

	regel = maakLijst(10, sizeof(char*));

	gegeven_hoeken_p = maakLijst(10, sizeof(Vec3f));
	gegeven_hoeken_v = maakLijst(10, sizeof(Vec2f));
	gegeven_hoeken_n = maakLijst(10, sizeof(Vec3f));

	hoeken_p = maakLijst(10, sizeof(Vec3f));
	hoeken_v = maakLijst(10, sizeof(Vec2f));
	hoeken_n = maakLijst(10, sizeof(Vec3f));
	hoekentallen = maakLijst(10, sizeof(Hoektallen));
	vlakken = maakLijst(10, sizeof(Vlak));

	while (!EOF_gevonden) {
		// Leidende Ruimte
		char* woord = leesWoord();
		if (woord == NULL) continue;

		// Aantekeningen
		if (strcmp(woord, "#") == 0) {
			free(woord);
			verwerpRegel();
		} else if (strcmp(woord, "v") == 0) {
			free(woord);
			leesHoekP();
		} else if (strcmp(woord, "vt") == 0) {
			free(woord);
			leesHoekV();
		} else if (strcmp(woord, "vn") == 0) {
			free(woord);
			leesHoekN();
		} else if (strcmp(woord, "f") == 0) {
			free(woord);
			leesVlak();
		} else {
			free(woord);
			fprintf(stderr, "Onbekend begin van obj regel: %s\n", woord);
			if (!regeleind_gevonden) verwerpRegel();
		}
	}

	fputs("[", stdout);
	for (unsigned int i = 0; i < hoeken_n->tel; i++) {
		Vec3f v = lijstKrijg(hoeken_n, i, Vec3f);
		printf("<%f, %f, %f>, ", v.x, v.y, v.z);
	}
	fputs("]\n[", stdout);
	for (unsigned int i = 0; i < vlakken->tel; i++) {
		Vlak v = lijstKrijg(vlakken, i, Vlak);
		printf("<%u, %u, %u>, ", v.hoek1, v.hoek2, v.hoek3);
	}
	fputs("]\n", stdout);

	Vorm* vorm = maakVorm(hoeken_p, vlakken);
	vormVoegInhoudToe(vorm, hoeken_v, 1);
	vormVoegInhoudToe(vorm, hoeken_n, 2);

	fclose(bestand);
	verwijderLijst(regel, waar);

	verwijderLijst(gegeven_hoeken_p, onwaar);
	verwijderLijst(gegeven_hoeken_v, onwaar);
	verwijderLijst(gegeven_hoeken_n, onwaar);

	verwijderLijst(hoeken_p, onwaar);
	verwijderLijst(hoeken_v, onwaar);
	verwijderLijst(hoeken_n, onwaar);
	verwijderLijst(hoekentallen, onwaar);
	verwijderLijst(vlakken, onwaar);
	return vorm;
}
