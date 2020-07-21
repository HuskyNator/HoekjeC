#include "bestandslezer.h"

#include "lijst.h"
#include "lineair.h"
#include "materiaal.h"
#include "vorm.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

typedef struct bestand Bestand;
struct bestand {
	FILE* bestand;
	Lijst* regel;  // char*[]
	booleaan EOF_gevonden;
	booleaan regeleind_gevonden;
};

static Bestand obj_bestand;
static Bestand mtl_bestand;

static Lijst* gegeven_hoeken_p;	 // Vec3f[]
static Lijst* gegeven_hoeken_v;	 // Vec2f[]
static Lijst* gegeven_hoeken_n;	 // Vec3f[]

static Lijst* hoeken_p;		 // Vec3f[]
static Lijst* hoeken_v;		 // Vec2f[]
static Lijst* hoeken_n;		 // Vec3f[]
static Lijst* hoekentallen;	 // Hoektallen[]
static Lijst* vlakken;		 // Vlak[]

static char verwerpRuimte(Bestand* bestand) {
	char teken = getc(bestand);
	while (teken == ' ' || teken == '\t' || teken == '\r' /*|| teken == '\n'*/) {
		teken = getc(bestand);
	}
	return teken;
}

static void verwerpRegel(Bestand* bestand) {
	char teken;
	while (teken != '\n' && teken != EOF) {
		teken = getc(bestand);
	}
}

static char* leesWoord(Bestand* bestand) {
	unsigned char woordTel = 0;
	unsigned char woordGrootte = 10;
	bestand->regeleind_gevonden = onwaar;
	char* woord = malloc(woordGrootte + 1);
	char teken = verwerpRuimte(bestand);
	while (teken != ' ' && teken != '\n' && teken != '\t' && teken != '\r' && teken != EOF) {
		if (teken == '#') {
			verwerpRegel(bestand);
			break;
		}
		if (woordTel == woordGrootte) {
			woordGrootte += 5;
			woord = realloc(woord, woordGrootte + 1);
		}
		woord[woordTel] = teken;
		woordTel++;
		teken = getc(bestand->bestand);
	}
	if (teken == EOF) bestand->EOF_gevonden = waar;
	else if (teken == '\n')
		bestand->regeleind_gevonden = waar;
	if (woordTel == 0) {
		free(woord);
		return NULL;
	}
	woord[woordTel] = '\0';
	return woord;
}

/**
 * Leest een regel van een bestand
 */
static Lijst* leesRegel(Bestand* bestand) {
	Lijst* regel = maakLijst(10, sizeof(char*));
	char* woord = leesWoord(bestand);
	while (woord != NULL) {
		lijstVoeg(bestand->regel, &woord);
		if (bestand->EOF_gevonden || bestand->regeleind_gevonden) break;
		woord = leesWoord(bestand);
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
	leesRegel(&obj_bestand);
	if (obj_bestand.regel->tel < 3 || obj_bestand.regel->tel > 4) {
		fprintf(stderr, "Verwachtte 3/4 woorden in .obj hoek(p) regel maar kreeg: %d\n", obj_bestand.regel->tel);
		exit(-1);
	}
	float x = strtof(lijstKrijg(obj_bestand.regel, 0, char*), NULL);
	float y = strtof(lijstKrijg(obj_bestand.regel, 1, char*), NULL);
	float z = strtof(lijstKrijg(obj_bestand.regel, 2, char*), NULL);
	float w = 1;
	if (obj_bestand.regel->tel == 4) w = strtof(lijstKrijg(obj_bestand.regel, 3, char*), NULL);
	Vec4f v = (Vec4f){x, y, z, w};
	Vec3f v2 = Vec4n3f(v, onwaar);
	lijstVoeg(gegeven_hoeken_p, &v2);
}

static void leesHoekN() {
	leesRegel(&obj_bestand);
	if (obj_bestand.regel->tel != 3) {
		fprintf(stderr, "Verwachtte 3 woorden in .obj hoek(n) regel maar kreeg: %d\n", obj_bestand.regel->tel);
		exit(-1);
	}
	float x = strtof(lijstKrijg(obj_bestand.regel, 0, char*), NULL);
	float y = strtof(lijstKrijg(obj_bestand.regel, 1, char*), NULL);
	float z = strtof(lijstKrijg(obj_bestand.regel, 2, char*), NULL);
	Vec3f v = Vec3fn((Vec3f){x, y, z});
	lijstVoeg(gegeven_hoeken_n, &v);
}

static void leesHoekV() {
	leesRegel(&obj_bestand);
	if (obj_bestand.regel->tel != 2) {
		fprintf(stderr, "Verwachtte 2 woorden in .obj hoek(v) regel maar kreeg: %d\n", obj_bestand.regel->tel);
		exit(-1);
	}
	float x = strtof(lijstKrijg(obj_bestand.regel, 0, char*), NULL);
	float y = strtof(lijstKrijg(obj_bestand.regel, 1, char*), NULL);
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
	leesRegel(&obj_bestand);
	if (obj_bestand.regel->tel < 3) {
		fprintf(stderr, "Fout aantal hoeken in vlak: %d", obj_bestand.regel->tel);
		exit(-1);
	}

	Hoektallen oorsprong = leesHoektallen(lijstKrijg(obj_bestand.regel, 0, char*));
	unsigned int oorsprongtal = voegHoekToe(&oorsprong);

	Hoektallen tweede = leesHoektallen(lijstKrijg(obj_bestand.regel, 1, char*));
	unsigned int tweedetal = voegHoekToe(&tweede);

	for (int i = 2; i < obj_bestand.regel->tel; i++) {
		Hoektallen derde = leesHoektallen(lijstKrijg(obj_bestand.regel, i, char*));
		unsigned int derdetal = voegHoekToe(&derde);

		Vlak vlak = (Vlak){oorsprongtal, tweedetal, derdetal};
		lijstVoeg(vlakken, &vlak);
		// tweede = derde;
		tweedetal = derdetal;
	}
}

// TODO gebruik doorlees stukken voor te grote bestanden
Vorm* leesObj(const char* bestandsnaam) {
	FILE* bestand = fopen(bestandsnaam, "rb");
	if (bestand == NULL) {
		fprintf(stderr, "Bestand '%s' bestaat niet.", bestandsnaam);
		return NULL;
	}

	obj_bestand = (Bestand){.bestand = bestand,
							.regel = maakLijst(10, sizeof(char*)),
							.EOF_gevonden = onwaar,
							.regeleind_gevonden = onwaar};

	gegeven_hoeken_p = maakLijst(10, sizeof(Vec3f));
	gegeven_hoeken_v = maakLijst(10, sizeof(Vec2f));
	gegeven_hoeken_n = maakLijst(10, sizeof(Vec3f));

	hoeken_p = maakLijst(10, sizeof(Vec3f));
	hoeken_v = maakLijst(10, sizeof(Vec2f));
	hoeken_n = maakLijst(10, sizeof(Vec3f));
	hoekentallen = maakLijst(10, sizeof(Hoektallen));
	vlakken = maakLijst(10, sizeof(Vlak));

	while (!obj_bestand.EOF_gevonden) {
		// Leidende Ruimte
		char* woord = leesWoord(bestand);
		if (woord == NULL) continue;

		// Aantekeningen
		if (strcmp(woord, "#") == 0) {
			verwerpRegel(bestand);
		} else if (strcmp(woord, "v") == 0) {
			leesHoekP(bestand);
		} else if (strcmp(woord, "vt") == 0) {
			leesHoekV(bestand);
		} else if (strcmp(woord, "vn") == 0) {
			leesHoekN(bestand);
		} else if (strcmp(woord, "f") == 0) {
			leesVlak(bestand);
		} else if (strcmp(woord, "usemtl") == 0) {
			char* mtlNaam = leesWoord(bestand);
			for (int i = 0; i < materialen->tel; i++) {
				// TODO
			}
		} else if (strcmp(woord, "mtllib") == 0) {
			char* mtlBestandsnaam = leesWoord(bestand);
			leesMtl(mtlBestandsnaam);  // TODO
			free(mtlBestandsnaam);
		} else {
			fprintf(stderr, "Onbekend begin van obj regel: %s\n", woord);
			if (!obj_bestand.regeleind_gevonden) verwerpRegel(bestand);
		}
		free(woord);
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
	verwijderLijst(obj_bestand.regel, waar);

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

Lijst* materialen;	// Materiaal[]
Materiaal huidig_materiaal;

static void leesKa() {
	leesRegel(&mtl_bestand);
	if (mtl_bestand.regel->tel == 0) {
		fputs("Ka vereist argumenten maar bevatte er 0.", stderr);
		return;
	}

	char* eerste = lijstKrijg(mtl_bestand.regel, 0, char*);
	if (strstr(eerste, "spectral") == 0) {	 /*TODO*/
	} else if (strstr(eerste, "xyz") == 0) { /*TODO*/
	} else {
		float r = strtof(eerste, NULL);
		if (mtl_bestand.regel->tel == 1) {
			huidig_materiaal.afweer_kleur = (Vec3f){r, r, r};
		} else {
			char* tweede = lijstKrijg(mtl_bestand.regel, 1, char*);
			char* derde = lijstKrijg(mtl_bestand.regel, 2, char*);
			float g = strtof(tweede, NULL);
			float b = strtof(derde, NULL);
			huidig_materiaal.afweer_kleur = (Vec3f){r, g, b};
		}
	}
}

Lijst* leesMtl(const char* bestandsnaam) {
	FILE* bestand = fopen(bestandsnaam, "rb");
	if (bestand == NULL) {
		fprintf(stderr, "Bestand '%s' bestaat niet.", bestandsnaam);
		return NULL;
	}

	mtl_bestand = (Bestand){.bestand = bestand,
							.regel = maakLijst(10, sizeof(char*)),
							.EOF_gevonden = onwaar,
							.regeleind_gevonden = onwaar};

	materialen = maakLijst(4, sizeof(Materiaal));

	while (!mtl_bestand.EOF_gevonden) {
		char* woord = leesWoord(bestand);
		if (strcmp(woord, "newmtl") == 0) {
			huidig_materiaal = (Materiaal){leesWoord(bestand)};
		} else if (strcmp(woord, "Ka") == 0) {
			leesKa();
		} else if (strcmp(woord, "Kd") == 0) {
		} else if (strcmp(woord, "Ks") == 0) {
		} else if (strcmp(woord, "Ns") == 0) {
		} else if (strcmp(woord, "d") == 0) {  // TODO of Tr?
		} else if (strcmp(woord, "Ni") == 0) {
		} else if (strcmp(woord, "illum") == 0) {
		} else if (strcmp(woord, "map_Ka") == 0) {
		} else if (strcmp(woord, "map_Kd") == 0) {
		} else if (strcmp(woord, "map_Ks") == 0) {
		} else if (strcmp(woord, "map_Ns") == 0) {
		} else if (strcmp(woord, "map_d") == 0) {
		}
	}
