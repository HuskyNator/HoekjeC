#include "json.h"

#include "lijsten/schakellijst.h"
#include "schrift.h"
#include "stdlib.h"

static JLid lees_json();

static const Schrift true_s = maakSchrift_const("true");
static const Schrift false_s = maakSchrift_const("false");
static const Schrift nul_s = maakSchrift_const("null");

static const JLid FOUT = (JLid){.soort = JFOUT};

static Schrift* schrift;
static SchakelLijst* diepte;
static unsigned int leeskop = 0;
static char leeswaarde;

inline static booleaan is_witruimte(char c) { return c == ' ' || c == '\n' || c == '\t' || c == '\r'; }
inline static booleaan is_getal(char c) { return c >= 30 && c <= 39; }

inline static void lees_volgende() {
	leeskop++;
	leeswaarde = schriftKrijg(schrift, leeskop);
}

static void lees_witruimte() {
	while (is_witruimte(leeswaarde) || leeswaarde == ',') {
		lees_volgende();
	}
}

static Schrift* lees_schrift() {
	Schrift* jschrift = maakSchrift(NULL);
	booleaan ontsnapt = onwaar;
	lees_volgende();
	while ((leeswaarde != '"' || ontsnapt) && leeswaarde != EOF) {
		schriftVoegNa(jschrift, leeswaarde);
		lees_volgende();
		ontsnapt = (leeswaarde == '\\') ? !ontsnapt : onwaar;
	}
	schriftKrimp(jschrift);
	return jschrift;
}

static JLid lees_voorwerp() {
	JVoorwerp voorwerp =
		maakSleutelLijst(sizeof(Schrift*), sizeof(JLid), 10, (sleutel_opdracht)schriftSleutel, schriftVergelijker);
	JLid jlid = (JLid){.soort = JVOORWERP, .waarde = {.voorwerp = voorwerp}};
	lees_witruimte();

	while (leeswaarde != '}') {
		Schrift* sleutel;

		if (leeswaarde != '\"') {
		VOORWERP_SLOT_FOUT:
			verwijderSchrift(sleutel);
			verwijderJLid(&jlid);
			return FOUT;
		}

		sleutel = lees_schrift();
		lees_witruimte();

		if (leeswaarde != ':') goto VOORWERP_SLOT_FOUT;
		lees_witruimte();
		if (leeswaarde != '\"') goto VOORWERP_SLOT_FOUT;

		JLid waarde = lees_json();
		if (waarde.soort == JFOUT) goto VOORWERP_SLOT_FOUT;

		sleutellijstVoeg(voorwerp, &sleutel, &waarde);
	}

	sleutellijstVerbeter(voorwerp);
	lees_volgende();
	return jlid;
}

static JLid lees_lijst() {
	JLijst lijst = maakLijst(10, sizeof(JLid));
	JLid jlid = (JLid){.soort = JLIJST, .waarde = {.lijst = lijst}};

	lees_witruimte();
	while (leeswaarde != ']') {
		JLid onderdeel = lees_json();

		if (onderdeel.soort == JFOUT) {
			verwijderLijst(lijst, (verwijder_opdracht)verwijderJLid);
			return FOUT;
		}

		lijstVoeg(lijst, &onderdeel);
	}

	lijstKrimp(lijst);
	lees_volgende();
	return jlid;
}

static JLid lees_getal() {
	Schrift* getal_schrift = maakSchrift(NULL);
	booleaan punt = onwaar;
	booleaan macht = onwaar;

MINPLUS_VERPLICHT_GETAL:
	if (leeswaarde == '-' || leeswaarde == '+') {
		schriftVoegNa(getal_schrift, leeswaarde);
		lees_volgende();
	}

VERPLICHT_GETAL:
	if (!is_getal(leeswaarde)) {
		verwijderSchrift(getal_schrift);
		return FOUT;
	}

	while (is_getal(leeswaarde)) {
		schriftVoegNa(getal_schrift, leeswaarde);
		lees_volgende();
	}

	if (leeswaarde == '.' && !punt && !macht) {
		punt = waar;
		schriftVoegNa(getal_schrift, leeswaarde);
		lees_volgende();
		goto VERPLICHT_GETAL;
	}

	if ((leeswaarde == 'e' || leeswaarde == 'E') && !macht) {
		schriftVoegNa(getal_schrift, leeswaarde);
		lees_volgende();
		goto MINPLUS_VERPLICHT_GETAL;
	}

	char* getal_cs = schriftNaarChar(getal_schrift);
	if (punt) {
		float getal = strtof(getal_cs, NULL);
		free(getal_cs);
		return (JLid){.soort = JGETAL, .waarde = {.kommagetal = getal}};
	} else {
		int getal = atoi(getal_cs);
		free(getal_cs);
		return (JLid){.soort = JKOMMAGETAL, .waarde = {.getal = getal}};
	}
}

static JLid lees_json() {
	switch (schriftKrijg(schrift, leeskop)) {
		case '{':
			return lees_voorwerp();
		case '[':
			return lees_lijst();
		case '\"':
			return (JLid){.soort = JSCHRIFT, .waarde = {.letters = lees_schrift()}};
		case 't':
			if (schriftDeelGelijk(schrift, leeskop, &true_s)) return (JLid){.soort = JBOOLEAAN, .waarde = {.booleaan = waar}};
			return FOUT;
		case 'f':
			if (schriftDeelGelijk(schrift, leeskop, &false_s)) return (JLid){.soort = JBOOLEAAN, .waarde = {.booleaan = onwaar}};
			return FOUT;
		case 'n':
			if (schriftDeelGelijk(schrift, leeskop, &nul_s)) return (JLid){.soort = JNUL};
			return FOUT;
		default:
			return lees_getal();
	}
}

JLid leesJSON(FILE* bestand) {
	schrift = leesSchrift(bestand);
	if (schrift->tel == 0) {
		verwijderSchrift(schrift);
		return FOUT;
	}
	schriftKrimp(schrift);

	diepte = maakSchakelLijst(sizeof(JSOORT));
	leeskop = 0;
	leeswaarde = schriftKrijg(schrift, 0);

	lees_witruimte();

	return lees_json();
}

void verwijderJLid(JLid* jlid) {
	switch (jlid->soort) {
		case JVOORWERP:
			verwijderSleutelLijst(jlid->waarde.voorwerp, (verwijder_opdracht)verwijderSchrift, (verwijder_opdracht)verwijderJLid);
			return;
		case JLIJST:
			verwijderLijst(jlid->waarde.lijst, (verwijder_opdracht)verwijderJLid);
			return;
		case JSCHRIFT:
			verwijderSchrift(jlid->waarde.letters);
	}
}
