#include "bestandslezer.h"

#include "lijsten/lijst.h"
#include "lijsten/sleutellijst.h"
#include "wiskunde/lineair.h"
#include "verf/materiaal.h"
#include "voorwerpen/vorm.h"

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

typedef struct hoektallen Hoektallen;
struct hoektallen {
	unsigned int plek;
	unsigned int verf;
	unsigned int normaal;
};

// OBJ

#define HOEK_PER_BYTE 0.025

static Bestand obj_bestand;

static Lijst* gegeven_hoeken_p;					// Vec3f[]
static Lijst* gegeven_hoeken_v;					// Vec2f[]
static Lijst* gegeven_hoeken_n;					// Vec3f[]
static Lijst* hoeken_p;							// Vec3f[]
static Lijst* hoeken_v;							// Vec2f[]
static Lijst* hoeken_n;							// Vec3f[]
static Lijst* hoekentallen;						// Hoektallen[]
static SleutelLijst* hoekentallen_plek_sloten;	// <Hoektallen, unsigned int>[]
static Lijst* vlakken;							// Vlak[]
static Lijst* materialen;						// Materiaal[]
static Lijst* groepen;							// VlakGroep[]

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
	char teken = '\0';
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
	unsigned int tel = 0;
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
	float x = (float)atof(lijstKrijg(obj_bestand.regel, 0, char*));
	float y = (float)atof(lijstKrijg(obj_bestand.regel, 1, char*));
	float z = (float)atof(lijstKrijg(obj_bestand.regel, 2, char*));
	float w = 1;
	if (obj_bestand.regel->tel == 4) w = (float)atof(lijstKrijg(obj_bestand.regel, 3, char*));
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
	float x = (float)atof(lijstKrijg(obj_bestand.regel, 0, char*));
	float y = (float)atof(lijstKrijg(obj_bestand.regel, 1, char*));
	float z = (float)atof(lijstKrijg(obj_bestand.regel, 2, char*));
	Vec3f v = Vec3fn((Vec3f){x, y, z});
	lijstVoeg(gegeven_hoeken_n, &v);
}

static void leesHoekV() {
	leesRegel(&obj_bestand);
	if (obj_bestand.regel->tel != 2) {
		fprintf(stderr, "Verwachtte 2 woorden in .obj hoek(v) regel maar kreeg: %d\n", obj_bestand.regel->tel);
		exit(-1);
	}
	float x = (float)atof(lijstKrijg(obj_bestand.regel, 0, char*));
	float y = (float)atof(lijstKrijg(obj_bestand.regel, 1, char*));
	Vec2f v = (Vec2f){x, y};
	lijstVoeg(gegeven_hoeken_v, &v);
}

static Hoektallen leesHoektallen(char* hoektallen) {
	Hoektallen h = {0, 0, 0};
	char* getal_eind;
	booleaan dubbel = strstr(hoektallen, "//") != NULL;
	vervang(hoektallen, '/', ' ');
	const unsigned int p = strtoul(hoektallen, &getal_eind, 10);
	if (p == 0) return h;
	else if (p < 0)
		h.plek = gegeven_hoeken_p->tel + p;
	else
		h.plek = p - 1;

	if (!dubbel) {
		const unsigned int v = strtoul(getal_eind, &getal_eind, 10);
		if (v == 0) return h;
		else if (v < 0)
			h.verf = gegeven_hoeken_v->tel + v;
		else
			h.verf = v - 1;
	}

	const unsigned int n = strtoul(getal_eind, NULL, 10);
	if (n == 0) return h;
	else if (n < 0)
		h.normaal = gegeven_hoeken_n->tel + n;
	else
		h.normaal = n - 1;
	return h;
}

static unsigned int voegHoekToe(Hoektallen* hoektallen) {
	unsigned int oude;
	if (sleutellijstVind(hoekentallen_plek_sloten, hoektallen, &oude)) return oude;
	Vec3f p = lijstKrijg(gegeven_hoeken_p, hoektallen->plek, Vec3f);
	Vec2f v = lijstKrijg(gegeven_hoeken_v, hoektallen->verf, Vec2f);
	Vec3f n = lijstKrijg(gegeven_hoeken_n, hoektallen->normaal, Vec3f);
	lijstVoeg(hoeken_p, &p);
	lijstVoeg(hoeken_v, &v);
	lijstVoeg(hoeken_n, &n);
	unsigned int plek = lijstVoeg(hoekentallen, hoektallen);
	sleutellijstVoeg(hoekentallen_plek_sloten, hoektallen, &plek);
	return plek;
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

	for (unsigned int i = 2; i < obj_bestand.regel->tel; i++) {
		Hoektallen derde = leesHoektallen(lijstKrijg(obj_bestand.regel, i, char*));
		unsigned int derdetal = voegHoekToe(&derde);

		Vlak vlak = (Vlak){oorsprongtal, tweedetal, derdetal};
		lijstVoeg(vlakken, &vlak);
		// tweede = derde;
		tweedetal = derdetal;
	}
}

static void leesMaterialen(Lijst* materialen, const char* parent_dir) {
	leesRegel(&obj_bestand);
	if (obj_bestand.regel->tel == 0) {
		fputs("Vewachtte materiaal bestandsnamen.", stderr);
		return;
	}
	for (unsigned int i = 0; i < obj_bestand.regel->tel; i++) {
		char* mtl_file = lijstKrijg(obj_bestand.regel, i, char*);
		char* mtl_path = strConcat(parent_dir, mtl_file);

		leesMtl(mtl_path, materialen);

		free(mtl_path);
	}
}

static void gebruikAnderMateriaal() {
	leesRegel(&obj_bestand);
	if (obj_bestand.regel->tel != 1) {
		fprintf(stderr, "Verwachtte enkele naam voor 'usemtl' maar kreeg er: %u\n", obj_bestand.regel->tel);
		return;
	}
	char* mtlNaam = lijstKrijg(obj_bestand.regel, 0, char*);

	booleaan gevonden = onwaar;
	unsigned int materiaal_tal;
	for (unsigned int i = 0; i < materialen->tel; i++) {
		Materiaal* m = &lijstKrijg(materialen, i, Materiaal);
		if (strcmp(m->naam, mtlNaam) == 0) {
			gevonden = waar;
			materiaal_tal = i;
			break;
		}
	}
	if (!gevonden) {
		#ifndef NDEBUG
		fprintf(stderr, "Verzocht materiaal '%s' is niet gevonden.\n", mtlNaam);
		#endif
		return;
	}

	// VlakGroep grootte nog onbekend, echter begint het vanaf de huidige vlakken grootte.
	VlakGroep vg = {.grootte = vlakken->tel, .materiaal_tal = materiaal_tal};
	lijstVoeg(groepen, &vg);

	// De grootte van de vorige VlakGroep is nu wel bekend.
	if (groepen->tel > 1) {
		VlakGroep* vorige = &lijstKrijg(groepen, groepen->tel - 2, VlakGroep);
		vorige->grootte = vlakken->tel - vorige->grootte;
	}
}

static unsigned int hoektallen_sleutelaar(Hoektallen* hoektallen) {
	unsigned int sleutel_plek = hoektallen->normaal;
	sleutel_plek *= 37;
	sleutel_plek += hoektallen->verf;
	sleutel_plek *= 37;
	sleutel_plek += hoektallen->normaal;
	return sleutel_plek;
}

char* strConcat(const char* first, const char* second){
	char* concat = malloc(strlen(first)+strlen(second)+1);
	strcpy(concat, first);
	strcat(concat,second);
	return concat;
}

char* findDir(const char* filePath){
	size_t path_length = strlen(filePath);
	if(path_length == 0){
		char* parent_dir = malloc(2);
		parent_dir[0] = '.';
		parent_dir[1] = '\0';
		return parent_dir;
	}

	char* parent_dir = malloc(path_length + 1);
	strcpy(parent_dir, filePath);
	int last_slash = 0;
	for(int i=strlen(filePath);i>0; i--){
		if(filePath[i] == '/' || filePath[i] == '\\'){
			last_slash = i;
			break;
		}
	}
	parent_dir[last_slash+1] = '\0';
	if(last_slash == 0){
		parent_dir[0] = '.';
	}
	return parent_dir;
}

Vorm* leesObj(const char* bestandsnaam) {
	FILE* bestand = fopen(bestandsnaam, "rb");
	if (bestand == NULL) {
		fprintf_s(stderr, "Bestand '%s' bestaat niet.\n", bestandsnaam);
		return NULL;
	}

	char* parent_dir = findDir(bestandsnaam);

	fseek(bestand, 0, SEEK_END);
	long grootte = ftell(bestand);
	fseek(bestand, 0, SEEK_SET);
	int geschat_aantal_hoeken = (int)ceilf(grootte * HOEK_PER_BYTE);

	obj_bestand = (Bestand){.bestand = bestand,
							.regel = maakLijst(10, sizeof(char*)),
							.EOF_gevonden = onwaar,
							.regeleind_gevonden = onwaar};

	gegeven_hoeken_p = maakLijst(10, sizeof(Vec3f));
	gegeven_hoeken_v = maakLijst(10, sizeof(Vec2f));
	gegeven_hoeken_n = maakLijst(10, sizeof(Vec3f));
	hoekentallen = maakLijst(10, sizeof(Hoektallen));
	hoekentallen_plek_sloten = maakSleutelLijst(sizeof(Hoektallen), sizeof(unsigned int), geschat_aantal_hoeken,
												(sleutel_opdracht)hoektallen_sleutelaar);

	hoeken_p = maakLijst(10, sizeof(Vec3f));
	hoeken_v = maakLijst(10, sizeof(Vec2f));
	hoeken_n = maakLijst(10, sizeof(Vec3f));
	vlakken = maakLijst(10, sizeof(Vlak));
	materialen = maakLijst(4, sizeof(Materiaal));
	groepen = maakLijst(4, sizeof(VlakGroep));

	lijstVoeg(materialen, standaard_materiaal());

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
			// } else if (strcmp(woord, "g") == 0) {  // TODO
			// } else if (strcmp(woord, "o") == 0) {  // TODO
		} else if (strcmp(woord, "usemtl") == 0) {
			gebruikAnderMateriaal();
		} else if (strcmp(woord, "mtllib") == 0) {
			leesMaterialen(materialen, parent_dir);
		} else {
			#ifndef NDEBUG
			fprintf(stderr, "Onbekend begin van obj regel: %s\n", woord);
			#endif
			if (!obj_bestand.regeleind_gevonden) verwerpRegel(&obj_bestand);
		}
		free(woord);
	}

	if (groepen->tel == 0) {
		VlakGroep vg = (VlakGroep){.grootte = vlakken->tel, .materiaal_tal = 0};
		lijstVoeg(groepen, &vg);
	} else {
		VlakGroep* vg = &lijstKrijg(groepen, groepen->tel - 1, VlakGroep);
		vg->grootte = vlakken->tel - vg->grootte;
	}

	Vorm* vorm = maakVorm(hoeken_p, hoeken_v, hoeken_n, vlakken, groepen, materialen);
	vormVoegInhoudToe(vorm, hoeken_v, 1);
	vormVoegInhoudToe(vorm, hoeken_n, 2);

	fclose(bestand);
	verwijderLijst(obj_bestand.regel, waar);

	verwijderLijst(gegeven_hoeken_p, onwaar);
	verwijderLijst(gegeven_hoeken_v, onwaar);
	verwijderLijst(gegeven_hoeken_n, onwaar);
	verwijderLijst(hoekentallen, onwaar);

	verwijderSleutelLijst(hoekentallen_plek_sloten);

	free(parent_dir);
	return vorm;
}

static void leesK(Vec3f* doel) {
	leesRegel(&mtl_bestand);
	if (mtl_bestand.regel->tel == 0) {
		fputs("K(a/d/s) vereist 1+ argumenten maar bevatte er 0.\n", stderr);
		return;
	}

	char* eerste = lijstKrijg(mtl_bestand.regel, 0, char*);
	if (strcmp(eerste, "spectral") == 0) {
		fputs("'spectral' moet nog gemaakt worden.\n", stderr);	 // TODO
	} else if (strcmp(eerste, "xyz") == 0) {
		fputs("'xyz' moet nog gemaakt worden.\n", stderr);	// TODO
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
		fputs("Verwachtte getal maar kreeg niets.\n", stderr);
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
		return;
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
			Materiaal* nieuw_materiaal = malloc(sizeof(Materiaal));

			char* naam = leesWoord(&mtl_bestand);
			if (naam == NULL) {
				fputs("Mis naam van materiaal.\n", stderr);
				const char* missende_naam = "<missende naam>";
				naam = malloc(sizeof(missende_naam) + 1);
				strcpy(naam, missende_naam);
			}
			nieuw_materiaal->naam = naam;

			unsigned int plek = lijstVoeg(materialen, nieuw_materiaal);
			huidig_materiaal = &lijstKrijg(materialen, plek, Materiaal);
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
			#ifndef NDEBUG
			fprintf(stderr, "Onbekend begin van mtl regel: %s\n", woord);
			#endif
			if (!mtl_bestand.regeleind_gevonden) verwerpRegel(&mtl_bestand);
		}
	}
}

Materiaal* standaard_materiaal() {
	Materiaal* m = malloc(sizeof(Materiaal));
	const char* naam = "standaard";
	m->naam = malloc(strlen(naam) + 1);
	strcpy(m->naam, naam);
	m->vaste_kleur = (Vec3f){0.05, 0.05, 0.05};
	m->afweer_kleur = (Vec3f){1, 1, 1};
	m->weerkaats_kleur = (Vec3f){1, 1, 1};
	m->weerkaatsing = 200;
	m->doorzichtigheid = 0;
	m->brekingsgetal = 1;
	return m;
}