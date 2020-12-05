
#include "HC/lezers/json.h"

#include <stdio.h>
#include <stdlib.h>

char* test(int argc, char const* argv[]) {
	// Niks
	FILE* niks_bestand = fopen("hulpbronnen/json/niks.json", "r");
	Json niks_json = leesJson(niks_bestand);
	fclose(niks_bestand);
	if (niks_json.soort != JNIKS) return "Json leest niks niet goed.";

	// JFout
	FILE* fout_bestand = fopen("hulpbronnen/json/fout.json", "r");
	Json fout_json = leesJson(fout_bestand);
	fclose(fout_bestand);
	if (fout_json.soort != JFOUT) return "Json leest fout niet goed.";

	// JNull
	FILE* null_bestand = fopen("hulpbronnen/json/null.json", "r");
	Json null_json = leesJson(null_bestand);
	fclose(null_bestand);
	if (null_json.soort != JNULL) return "Json leest null niet goed.";

	// JBooleaan
	FILE* waar_bestand = fopen("hulpbronnen/json/true.json", "r");
	Json waar_json = leesJson(waar_bestand);
	fclose(waar_bestand);
	if (waar_json.soort != JBOOLEAAN || waar_json.waarde.booleaan != waar) return "Json leest waar niet goed.";

	FILE* onwaar_bestand = fopen("hulpbronnen/json/false.json", "r");
	Json onwaar_json = leesJson(onwaar_bestand);
	fclose(onwaar_bestand);
	if (onwaar_json.soort != JBOOLEAAN || onwaar_json.waarde.booleaan != onwaar) return "Json leest onwaar niet goed.";

	// JGetal
	Schrift* getal_bestandsplek = maakSchrift("hulpbronnen/json/");
	char getal[4];
	for (int i = -11; i <= 11; i++) {
		itoa(i, getal, 10);
		schriftVoegNaMeer(getal_bestandsplek, getal, 0);
		schriftVoegNaMeer(getal_bestandsplek, ".json", 6);	// ook '\0'

		schriftAfdrukken(getal_bestandsplek);

		FILE* getal_bestand = fopen(getal_bestandsplek->inhoud, "r");
		if (getal_bestand == NULL) return "Kon getalbestand niet openen.";

		Json getal_json = leesJson(getal_bestand);
		if (getal_json.soort != JGETAL) return "Json leest getal niet goed. (1)";
		if (getal_json.waarde.getal != i) return "Json leest getal niet goed. (2)";
		fclose(getal_bestand);
		schriftVerwijderReeks(getal_bestandsplek, 17, getal_bestandsplek->tel);
	}

	// JKommagetal

	FILE* macht = fopen("hulpbronnen/json/1e2.json", "r");
	if (macht == NULL) return "Kon kommagetalbestand niet openen.";
	Json machtgetal_json = leesJson(macht);
	if (machtgetal_json.soort != JKOMMAGETAL) return "Json leest macht getal niet goed. (1)";
	if (machtgetal_json.waarde.kommagetal != 100) return "Json leest macht getal niet goed. (2)";
	fclose(macht);

	FILE* kommagetal = fopen("hulpbronnen/json/12,224.json", "r");
	if (kommagetal == NULL) return "Kon kommagetalbestand niet openen.";
	Json kommagetal_json = leesJson(kommagetal);
	if (kommagetal_json.soort != JKOMMAGETAL) return "Json leest kommagetal niet goed. (1)";
	if (kommagetal_json.waarde.kommagetal != (double)12.224) return "Json leest kommagetal niet goed. (2)";
	fclose(kommagetal);

	// JSchrift

	FILE* schrift_bestand = fopen("hulpbronnen/json/schrift.json", "r");
	if (schrift_bestand == NULL) return "Kon schriftbestand niet openen.";
	Json schrift_json = leesJson(schrift_bestand);
	if (schrift_json.soort != JSCHRIFT) return "Json lees schrift niet goed. (1)";

	Schrift schrift_const = maakSchrift_const("distels");
	if (!schriftGelijk(schrift_json.waarde.schrift, &schrift_const)) return "Json leest schrift niet goed. (2)";

	// JLijst

	FILE* lijst_bestand = fopen("hulpbronnen/json/lijst.json", "r");
	if (lijst_bestand == NULL) return "Kon lijstbestand niet openen.";
	Json lijst_json = leesJson(lijst_bestand);
	if (lijst_json.soort != JLIJST) return "Json leest lijst niet goed. (1)";
	JLijst lijst = lijst_json.waarde.lijst;
	if (lijst->tel != 5) return "Json leest lijst niet goed. (2)";
	Json lijst_onderdeel;
	lijst_onderdeel = *lijstKrijg(lijst, 0, Json);
	if (lijst_onderdeel.soort != JGETAL || lijst_onderdeel.waarde.getal != 1) return "Json leest lijst niet goed. (3)";
	lijst_onderdeel = *lijstKrijg(lijst, 1, Json);
	if (lijst_onderdeel.soort != JGETAL || lijst_onderdeel.waarde.getal != 2) return "Json leest lijst niet goed. (4)";
	lijst_onderdeel = *lijstKrijg(lijst, 2, Json);
	Schrift lijst_onderdeel_schrift = maakSchrift_const("appelmoes");
	if (lijst_onderdeel.soort != JSCHRIFT || !schriftGelijk(lijst_onderdeel.waarde.schrift, &lijst_onderdeel_schrift))
		return "Json leest lijst niet goed. (5)";
	lijst_onderdeel = *lijstKrijg(lijst, 3, Json);
	if (lijst_onderdeel.soort != JBOOLEAAN || lijst_onderdeel.waarde.booleaan != waar) return "Json leest lijst niet goed. (6)";
	lijst_onderdeel = *lijstKrijg(lijst, 4, Json);
	if (lijst_onderdeel.soort != JNULL) return "Json leest lijst niet goed. (7)";

	// JVoorwerp
	FILE* voorwerp_bestand = fopen("hulpbronnen/json/voorwerp.json", "r");
	if (voorwerp_bestand == NULL) return "Kon voorwerpbestand niet openen.";
	Json voorwerp_json = leesJson(voorwerp_bestand);
	if (voorwerp_json.soort != JVOORWERP) return "Json leest voorwerp niet goed. (1)";
	if (voorwerp_json.waarde.voorwerp == NULL) return "Json leest voorwerp niet goed. (2)";
	JVoorwerp voorwerp = voorwerp_json.waarde.voorwerp;
	if (voorwerp->tel != 5) return "Json leest voorwerp niet goed. (3)";
	Json onderdeel = {.soort = JNIKS};
	booleaan gevonden = onwaar;
	Schrift onderwerp_schrift = maakSchrift_const("Onderwerp");
	Schrift onderwerp_verwacht = maakSchrift_const("Thom's boodschappen");
	gevonden = sleutellijstVind(voorwerp, &onderwerp_schrift, &onderdeel);
	if (!gevonden || onderdeel.soort != JSCHRIFT || !schriftGelijk(&onderwerp_verwacht, onderdeel.waarde.schrift))
		return "Json leest voorwerp niet goed. (4)";

	gevonden = onwaar;
	onderdeel = (Json){.soort = JNIKS};
	Schrift appelmoes_schrift = maakSchrift_const("Appelmoes");
	int appelmoes_verwacht = 1;
	gevonden = sleutellijstVind(voorwerp, &appelmoes_schrift, &onderdeel);
	if (!gevonden || onderdeel.soort != JGETAL || onderdeel.waarde.getal != appelmoes_verwacht) return "Json leest voorwerp niet goed. (5)";

	gevonden = onwaar;
	onderdeel = (Json){.soort = JNIKS};
	Schrift tomaat_schrift = maakSchrift_const("Tomaat");
	Schrift tomaat_verwacht = maakSchrift_const("Tomaat is vies");
	gevonden = sleutellijstVind(voorwerp, &tomaat_schrift, &onderdeel);
	if (!gevonden || onderdeel.soort != JSCHRIFT || !schriftGelijk(&tomaat_verwacht, onderdeel.waarde.schrift))
		return "Json leest voorwerp niet goed. (6)";

	gevonden = onwaar;
	onderdeel = (Json){.soort = JNIKS};
	Schrift depressie_schrift = maakSchrift_const("Depressie");
	booleaan depressie_verwacht = waar;
	gevonden = sleutellijstVind(voorwerp, &depressie_schrift, &onderdeel);
	if (!gevonden || onderdeel.soort != JBOOLEAAN || onderdeel.waarde.booleaan != depressie_verwacht)
		return "Json leest voorwerp niet goed. (7)";

	gevonden = onwaar;
	onderdeel = (Json){.soort = JNIKS};
	Schrift hekelonderwerpen_schrift = maakSchrift_const("Hekelonderwerpen");
	gevonden = sleutellijstVind(voorwerp, &hekelonderwerpen_schrift, &onderdeel);
	if (!gevonden || onderdeel.soort != JLIJST) return "Json leest voorwerp niet goed. (8)";
	JLijst hekelonderwerpen = onderdeel.waarde.lijst;
	if (hekelonderwerpen->tel != 3) return "Json leest voorwerp niet goed. (9)";
	Schrift paul_schrift = maakSchrift_const("Paul");
	onderdeel = *lijstKrijg(hekelonderwerpen, 0, Json);
	if (!onderdeel.soort == JSCHRIFT || !schriftGelijk(onderdeel.waarde.schrift, &paul_schrift))
		return "Json leest voorwerp niet goed. (10)";
	onderdeel = *lijstKrijg(hekelonderwerpen, 1, Json);
	if (!onderdeel.soort == JGETAL || onderdeel.waarde.getal != 15) return "Json leest voorwerp niet goed. (11)";
	onderdeel = *lijstKrijg(hekelonderwerpen, 2, Json);
	if (!onderdeel.soort == JNULL) return "Json leest voorwerp niet goed. (12)";

	return NULL;
}

int main(int argc, char const* argv[]) {
	char* fout = test(argc, argv);
	if (fout == NULL) return 0;
	puts(fout);
	return 1;
}
