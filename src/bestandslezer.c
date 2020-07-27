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
		fprintf(stderr, "Bestand '%s' bestaat niet.\n", bestandsnaam);
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

// OBJ

static Bestand obj_bestand;

static Lijst* gegeven_hoeken_p;	 // Vec3f[]
static Lijst* gegeven_hoeken_v;	 // Vec2f[]
static Lijst* gegeven_hoeken_n;	 // Vec3f[]
static Lijst* hoeken_p;			 // Vec3f[]
static Lijst* hoeken_v;			 // Vec2f[]
static Lijst* hoeken_n;			 // Vec3f[]
static Lijst* hoekentallen;		 // Hoektallen[]
static Lijst* vlakken;			 // Vlak[]

static Materiaal* huidig_materiaal;

// MTL

static Bestand mtl_bestand;

static char verwerpRuimte(Bestand* bestand) {
	char teken = getc(bestand->bestand);
	while (teken == ' ' || teken == '\t' || teken == '\r' /*|| teken == '\n'*/) {
		teken = getc(bestand->bestand);
	}
	return teken;
}

static void verwerpRegel(Bestand* bestand) {
	char teken;
	while (teken != '\n' && teken != EOF) {
		teken = getc(bestand->bestand);
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
static void leesRegel(Bestand* bestand) {
	lijstLeeg(bestand->regel, waar);
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
		fprintf(stderr, "Fout aantal hoeken in vlak: %d\n", obj_bestand.regel->tel);
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

static void leesMaterialen(Lijst* gegeven_materialen) {
	leesRegel(&obj_bestand);
	if (obj_bestand.regel->tel == 0) {
		fputs("Vewachtte materiaal bestandsnamen.", stderr);
		return;
	}
	for (unsigned int i = 0; i < obj_bestand.regel->tel; i++) {
		leesMtl(lijstKrijg(obj_bestand.regel, i, char*), gegeven_materialen);
	}
}

// TODO gebruik doorlees stukken voor te grote bestanden

// DeelVoorwerp[]
Lijst* leesObj(const char* bestandsnaam) {
	FILE* bestand = fopen(bestandsnaam, "rb");
	if (bestand == NULL) {
		fprintf(stderr, "Bestand '%s' bestaat niet.\n", bestandsnaam);
		return NULL;
	}

	Lijst* vormen = maakLijst(4, sizeof(Vorm*));

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

	Lijst* gegeven_materialen = maakLijst(4, sizeof(Materiaal*));  // Materiaal*[]
	Lijst* materialen = maakLijst(4, sizeof(Materiaal*));		   // Materiaal*[]
	Lijst* deel_plekken = maakLijst(4, sizeof(unsigned int));	   // unsigned int[]

	while (!obj_bestand.EOF_gevonden) {
		// Leidende Ruimte
		char* woord = leesWoord(&obj_bestand);
		if (woord == NULL) continue;

		// Aantekeningen, leesWoord geeft simpelweg NULL
		// if (woord[0] == '#') {
		// 	verwerpRegel(bestand);
		// } else
		if (strcmp(woord, "v") == 0) {
			leesHoekP();
		} else if (strcmp(woord, "vt") == 0) {
			leesHoekV();
		} else if (strcmp(woord, "vn") == 0) {
			leesHoekN();
		} else if (strcmp(woord, "f") == 0) {
			leesVlak();
		} else if (strcmp(woord, "g") == 0) {  // TODO
		} else if (strcmp(woord, "o") == 0) {  // TODO
		} else if (strcmp(woord, "usemtl") == 0) {
			leesRegel(&obj_bestand);
			if (obj_bestand.regel->tel != 1) {
				fprintf(stderr, "Verwachtte enkele naam voor 'usemtl' maar kreeg er: %u\n", obj_bestand.regel->tel);
				continue;
			}
			char* mtlNaam = lijstKrijg(obj_bestand.regel, 0, char*);
			Materiaal* huidig = NULL;
			for (unsigned int i = 0; i < gegeven_materialen->tel; i++) {
				Materiaal* m = lijstKrijg(gegeven_materialen, i, Materiaal*);
				if (strcmp(m->naam, mtlNaam) == 0) {
					huidig = m;
					break;
				}
			}
			if (huidig == NULL) {
				fprintf(stderr, "Verzochtte materiaal '%s' is niet gevonden.\n", mtlNaam);
				continue;
			}
			lijstVoeg(materialen, &huidig);
			lijstVoeg(deel_plekken, &vlakken->tel);
		} else if (strcmp(woord, "mtllib") == 0) {
			leesMaterialen(gegeven_materialen);
		} else {
			fprintf(stderr, "Onbekend begin van obj regel: %s\n", woord);
			if (!obj_bestand.regeleind_gevonden) verwerpRegel(&obj_bestand);
		}
		free(woord);
	}

	lijstVoeg(deel_plekken, &vlakken->tel);

	Lijst* deel_vormen = maakLijst(deel_plekken->tel, sizeof(VlakGroep));  // DeelVorm[]
	

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

	for (unsigned int i = 0; i < gegeven_materialen->tel; i++) {
		Materiaal* m = lijstKrijg(gegeven_materialen, i, Materiaal*);
		if (lijstVind(materialen, m, NULL) == onwaar) {
			free(m->naam);
			free(m);
		}
	}
	verwijderLijst(gegeven_materialen, onwaar);

	return vorm;
}

static void leesK(Vec3f* doel) {
	leesRegel(&mtl_bestand);
	if (mtl_bestand.regel->tel == 0) {
		fputs("K(a/d/s) vereist 1+ argumenten maar bevatte er 0.", stderr);
		return;
	}

	char* eerste = lijstKrijg(mtl_bestand.regel, 0, char*);
	if (strstr(eerste, "spectral") == 0) {
		fputs("'spectral' moet nog gemaakt worden.", stderr);  // TODO
	} else if (strstr(eerste, "xyz") == 0) {
		fputs("'xyz' moet nog gemaakt worden.", stderr);  // TODO
	} else {
		float r = strtof(eerste, NULL);
		if (mtl_bestand.regel->tel == 1) {
			*doel = (Vec3f){r, r, r};
		} else {
			char* tweede = lijstKrijg(mtl_bestand.regel, 1, char*);
			char* derde = lijstKrijg(mtl_bestand.regel, 2, char*);
			float g = strtof(tweede, NULL);
			float b = strtof(derde, NULL);
			*doel = (Vec3f){r, g, b};
		}
	}
}

void leesFloat(Bestand* bestand, float* getal) {
	char* woord = leesWoord(bestand);
	if (woord == NULL) {
		fputs("Verwachtte getal maar kreeg niets.", stderr);
		return;
	}
	*getal = strtof(woord, NULL);
}

void leesUChar(Bestand* bestand, unsigned char* getal) {
	char* woord = leesWoord(bestand);
	if (woord == NULL) {
		fputs("Verwachtte getal maar kreeg niets.", stderr);
		return;
	}
	*getal = (unsigned char)strtoul(woord, NULL, 10);
}

// Materiaal*[]
void leesMtl(const char* bestandsnaam, Lijst* materialen) {
	FILE* bestand = fopen(bestandsnaam, "rb");
	if (bestand == NULL) {
		fprintf(stderr, "Bestand '%s' bestaat niet.\n", bestandsnaam);
		return NULL;
	}

	mtl_bestand = (Bestand){.bestand = bestand,
							.regel = maakLijst(10, sizeof(char*)),
							.EOF_gevonden = onwaar,
							.regeleind_gevonden = onwaar};

	Materiaal* huidig_materiaal;

	while (!mtl_bestand.EOF_gevonden) {
		char* woord = leesWoord(&mtl_bestand);

		if (woord == NULL) continue;

		// Aantekeningen, leesWoord geeft simpelweg NULL
		// if (woord[0] == '#') verwerpRegel(&mtl_bestand);
		// else
		if (strcmp(woord, "newmtl") == 0) {
			char* naam = leesWoord(&mtl_bestand);
			if (naam == NULL) {
				fputs("Mis naam van materiaal.", stderr);
			}
			Materiaal* nieuw_materiaal = malloc(sizeof(Materiaal));
			nieuw_materiaal->naam = naam;
			unsigned int plek = lijstVoeg(materialen, &nieuw_materiaal);
			huidig_materiaal = lijstKrijg(materialen, plek, Materiaal*);
		} else if (strcmp(woord, "Ka") == 0) {
			leesK(&huidig_materiaal->vaste_kleur);
		} else if (strcmp(woord, "Kd") == 0) {
			leesK(&huidig_materiaal->afweer_kleur);
		} else if (strcmp(woord, "Ks") == 0) {
			leesK(&huidig_materiaal->weerkaats_kleur);
		} else if (strcmp(woord, "Ns") == 0) {
			leesFloat(&mtl_bestand, &huidig_materiaal->weerkaatsing);
		}
		// TODO Tf?
		else if (strcmp(woord, "d") == 0) {	 // TODO of Tr?
			leesFloat(&mtl_bestand, &huidig_materiaal->doorzichtigheid);
		} else if (strcmp(woord, "Ni") == 0) {
			leesFloat(&mtl_bestand, &huidig_materiaal->brekingsgetal);
		} else if (strcmp(woord, "illum") == 0) {
			leesUChar(&mtl_bestand, &huidig_materiaal->verlichtingswijze);
		}
		// } else if (strcmp(woord, "map_Ka") == 0) {	// TODO
		// } else if (strcmp(woord, "map_Kd") == 0) {	// TODO
		// } else if (strcmp(woord, "map_Ks") == 0) {	// TODO
		// } else if (strcmp(woord, "map_Ns") == 0) {	// TODO
		// } else if (strcmp(woord, "map_d") == 0) {	// TODO
		else {
			fprintf(stderr, "Onbekend begin van mtl regel: %s\n", woord);
			if (!obj_bestand.regeleind_gevonden) verwerpRegel(&mtl_bestand);
		}
	}
}
