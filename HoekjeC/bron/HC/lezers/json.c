#include "json.h"

#include "schrift.h"
#include "stdlib.h"

static Json lees_json();

static const Schrift true_s = maakSchrift_const("true");
static const Schrift false_s = maakSchrift_const("false");
static const Schrift null_s = maakSchrift_const("null");

static const Json FOUT = (Json){.soort = JFOUT};

static Schrift* schrift;
static unsigned int leeskop;
static booleaan uitgelezen;
static char leeswaarde;

inline static booleaan is_witruimte(char c) { return c == ' ' || c == '\n' || c == '\t' || c == '\r'; }
inline static booleaan is_getal(char c) { return c >= '0' && c <= '9'; }

inline static void lees_volgende() {
	leeskop++;
	if (leeskop == schrift->tel) {
		uitgelezen = waar;
		return;
	} else
		leeswaarde = schriftKrijg(schrift, leeskop);
}

static void lees_witruimte() {
	while (!uitgelezen && (is_witruimte(leeswaarde) || leeswaarde == ',')) {
		lees_volgende();
	}
}

static Schrift* lees_schrift() {
	Schrift* jschrift = maakSchrift(NULL);
	booleaan ontsnapt = onwaar;
	while (leeswaarde != '"' || ontsnapt) {
		if (uitgelezen) {
			verwijderSchrift(jschrift);
			return NULL;
		}
		schriftVoegNa(jschrift, leeswaarde);
		lees_volgende();
		ontsnapt = (leeswaarde == '\\') ? !ontsnapt : onwaar;
	}
	lees_volgende();
	schriftKrimp(jschrift);
	return jschrift;
}

static Json lees_voorwerp() {
	JVoorwerp voorwerp =
		maakSleutelLijst(sizeof(Schrift), sizeof(Json), 10, (sleutel_opdracht)schriftSleutel, (vergelijk_opdracht)schriftVergelijker);
	lees_witruimte();

	while (leeswaarde != '}') {
		Schrift* sleutel;

		if (leeswaarde != '\"') {
		VOORWERP_FOUT:
			verwijderSchrift(sleutel);
			verwijderSleutelLijst(voorwerp, (verwijder_opdracht)verwijderSchrift, (verwijder_opdracht)verwijderJson);
			return FOUT;
		}

		lees_volgende();
		sleutel = lees_schrift();
		if (sleutel == NULL) goto VOORWERP_FOUT;

		lees_witruimte();
		if (leeswaarde != ':') goto VOORWERP_FOUT;
		lees_volgende();
		lees_witruimte();

		Json waarde = lees_json();
		if (waarde.soort == JFOUT) goto VOORWERP_FOUT;
		lees_witruimte();

		sleutellijstVoeg(voorwerp, sleutel, &waarde);
	}

	voorwerp = sleutellijstVerbeter(voorwerp);
	Json jlid = (Json){.soort = JVOORWERP, .waarde = {.voorwerp = voorwerp}};
	lees_volgende();
	return jlid;
}

static Json lees_lijst() {
	JLijst lijst = maakLijst(10, sizeof(Json));
	Json jlid = (Json){.soort = JLIJST, .waarde = {.lijst = lijst}};

	lees_witruimte();
	while (leeswaarde != ']') {
		Json onderdeel = lees_json();
		lees_witruimte();

		if (onderdeel.soort == JFOUT || uitgelezen) {
			verwijderLijst(lijst, (verwijder_opdracht)verwijderJson);
			return FOUT;
		}

		lijstVoeg(lijst, &onderdeel);
	}

	lijstKrimp(lijst);
	lees_volgende();
	return jlid;
}

static Json lees_getal() {
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

	while (is_getal(leeswaarde) && !uitgelezen) {
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
		macht = waar;
		schriftVoegNa(getal_schrift, leeswaarde);
		lees_volgende();
		goto MINPLUS_VERPLICHT_GETAL;
	}

	char* getal_cs = schriftNaarChar(getal_schrift);
	if (punt || macht) {
		double getal = atof(getal_cs);
		free(getal_cs);
		return (Json){.soort = JKOMMAGETAL, .waarde = {.kommagetal = getal}};
	} else {
		int getal = atoi(getal_cs);
		free(getal_cs);
		return (Json){.soort = JGETAL, .waarde = {.getal = getal}};
	}
}

static Json lees_json() {
	if (uitgelezen) return FOUT;
	switch (leeswaarde) {
		case '{':
			lees_volgende();
			return lees_voorwerp();
		case '[':
			lees_volgende();
			return lees_lijst();
		case '\"':
			lees_volgende();
			return (Json){.soort = JSCHRIFT, .waarde = {.schrift = lees_schrift()}};
		case 't':
			if (!schriftDeelGelijk(schrift, leeskop, &true_s)) return FOUT;
			leeskop += 3;
			lees_volgende();
			return (Json){.soort = JBOOLEAAN, .waarde = {.booleaan = waar}};
		case 'f':
			if (!schriftDeelGelijk(schrift, leeskop, &false_s)) return FOUT;
			leeskop += 4;
			lees_volgende();
			return (Json){.soort = JBOOLEAAN, .waarde = {.booleaan = onwaar}};
		case 'n':
			if (!schriftDeelGelijk(schrift, leeskop, &null_s)) return FOUT;
			leeskop += 3;
			lees_volgende();
			return (Json){.soort = JNULL};
		default:
			return lees_getal();
	}
}

Json leesJson(FILE* bestand) {
	schrift = leesSchrift(bestand);
	if (schrift->tel == 0) {
		verwijderSchrift(schrift);
		return FOUT;
	}
	schriftKrimp(schrift);

	leeskop = 0;
	uitgelezen = onwaar;
	leeswaarde = schriftKrijg(schrift, 0);

	lees_witruimte();
	if (uitgelezen) return (Json){.soort = JNIKS};

	Json inhoud = lees_json();

	lees_witruimte();
	if (!uitgelezen) {
		verwijderJson(&inhoud);
		return FOUT;
	} else
		return inhoud;
}

void verwijderJson(Json* jlid) {
	switch (jlid->soort) {
		case JVOORWERP:
			verwijderSleutelLijst(jlid->waarde.voorwerp, (verwijder_opdracht)verwijderSchrift, (verwijder_opdracht)verwijderJson);
			return;
		case JLIJST:
			verwijderLijst(jlid->waarde.lijst, (verwijder_opdracht)verwijderJson);
			return;
		case JSCHRIFT:
			verwijderSchrift(jlid->waarde.schrift);
	}
}
