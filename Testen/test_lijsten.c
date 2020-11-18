#include "HC/lijsten/lijst.h"
#include "HC/schrift.h"

#include <stdio.h>

int verwijderde_waarde = 0;
void verwijder_opdracht_waarde(int* onderdeel) { verwijderde_waarde = *onderdeel; }

unsigned int verwijderde_tel = 0;
void verwijder_opdracht_tel(void* onderdeel) { verwijderde_tel++; }

booleaan vergelijk_opdracht_onwaar(const void* eerste, const void* tweede, size_t grootte) { return onwaar; }

booleaan vergelijk_opdracht_grootte(const void* eerste, const void* tweede, size_t grootte) { return grootte == sizeof(int); }

char* test_lijst(int argc, char** argv) {
	// MaakLijst
	Lijst* lijst = maakLijst(10, sizeof(int));
	if (lijst->grootte != 10 || lijst->inhoud == NULL || lijst->onderdeel_grootte != sizeof(int) || lijst->tel != 0)
		return "Kon lijst niet maken.";
	puts("[L] - MaakLijst werkt.");

	// LijstVoeg
	for (int i = 0; i < 10; i++) {
		lijstVoeg(lijst, &i);
	}
	if (lijst->grootte < 10 || lijst->tel != 10) return "Lijst voegt niet goed.";
	puts("[L] - LijstVoeg werkt.");

	// LijstKrijg (naar voren gehaald)
	for (int i = 0; i < 10; i++) {
		if (*lijstKrijg(lijst, i, int) != i) return "Lijst krijgt niet goed.";
	}
	puts("[L] - LijstKrijg werkt.");

	//	LijstVoegMeer
	const int i_lijst[] = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
	lijstVoegMeer(lijst, i_lijst, 10);
	if (lijst->grootte < 20 || lijst->tel != 20) return "Lijst voegt niet goed meervoudig. Grootte/Tel klopt niet.";
	for (int i = 0; i < 20; i++) {
		if (*lijstKrijg(lijst, i, int) != i) return "Lijst voegt niet goed meervoudig. Gevoegde waarden kloppen niet.";
	}
	puts("[L] - LijstVoegMeer werkt.");

	// LijstPlaats
	const int i_plaatsen[] = {-1, 100};
	lijstPlaats(lijst, 0, &i_plaatsen[0]);
	lijstPlaats(lijst, 2, &i_plaatsen[1]);
	if (lijst->grootte < 22 || lijst->tel != 22 || *lijstKrijg(lijst, 0, int) != -1 || *lijstKrijg(lijst, 1, int) != 0 ||
		*lijstKrijg(lijst, 2, int) != 100)
		return "Lijst plaatst niet goed.";

	for (int i = 3; i < 22; i++) {
		if (*lijstKrijg(lijst, i, int) != i - 2) return "Lijst plaatst niet goed.";
	}
	puts("[L] - LijstPlaats werkt.");

	// LijstPlaatsMeer
	lijstPlaatsMeer(lijst, 1, i_plaatsen, 2);
	if (lijst->grootte < 24 || lijst->tel != 24 || *lijstKrijg(lijst, 0, int) != -1 || *lijstKrijg(lijst, 1, int) != -1 ||
		*lijstKrijg(lijst, 2, int) != 100 || *lijstKrijg(lijst, 3, int) != 0 || *lijstKrijg(lijst, 4, int) != 100 ||
		*lijstKrijg(lijst, 5, int) != 1)
		return "Lijst plaatst niet goed meervoudig.";
	puts("[L] - LijstPlaatsMeer werkt.");

	// LijstVind
	unsigned int vind_plek = 0;
	booleaan gevonden = lijstVind(lijst, &i_lijst[0], vergelijk_opdracht_onwaar, &vind_plek);
	if (gevonden || vind_plek != 0) return "Lijst vindt iets met altijd onware vergelijker.";
	vind_plek = 1;
	gevonden = lijstVind(lijst, &i_lijst[0], vergelijk_opdracht_grootte, &vind_plek);
	if (!gevonden || vind_plek != 0) return "Lijst vindt niet met vergelijker die waar is als de gegeven grootte klopt.";
	gevonden = lijstVind(lijst, &i_lijst[0], geheugen_vergelijker, &vind_plek);
	if (!gevonden || vind_plek != 14) return "Lijst vindt niet goed.";
	puts("[L] - LijstVind werkt.");

	// LijstVerwijder
	booleaan verwijderd = lijstVerwijder(lijst, 1, NULL);
	if (!verwijderd || lijst->tel != 23 || *lijstKrijg(lijst, 1, int) != 100) return "Lijst verwijdert niet goed.";
	verwijderd = lijstVerwijder(lijst, 24, NULL);
	if (verwijderd || lijst->tel != 23) return "Lijst verwijdert mogelijk buiten haar bereik.";
	puts("[L] - LijstVerwijder werkt.");

	// LijstVindVerwijder
	const int vind_verwijder = 14;
	verwijderd = lijstVindVerwijder(lijst, &vind_verwijder, vergelijk_opdracht_onwaar, NULL);
	if (verwijderd || lijst->tel != 23 || *lijstKrijg(lijst, 17, int) != 14) return "Lijst vindverwijdert met onware vergelijker.";
	verwijderd = lijstVindVerwijder(lijst, &vind_verwijder, geheugen_vergelijker, NULL);
	if (!verwijderd || lijst->tel != 22 || *lijstKrijg(lijst, 17, int) != 15) return "Lijst vindverwijdert niet goed.";
	lijstVoeg(lijst, &vind_verwijder);
	verwijderd = lijstVindVerwijder(lijst, &vind_verwijder, geheugen_vergelijker, (verwijder_opdracht)verwijder_opdracht_waarde);
	if (!verwijderd || lijst->tel != 22 || verwijderde_waarde != 14) return "Lijst verwijder_opdracht werkt niet.";
	puts("[L] - LijstVindVerwijder werkt.");

	// LijstLeeg
	const int oude_tel = lijst->tel;
	lijstLeeg(lijst, verwijder_opdracht_tel);
	if (lijst->tel != 0 || verwijderde_tel != oude_tel) return "Lijst leegt niet goed.";
	puts("[L] - LijstLeeg werkt.");

	// LijstGroei
	const int oude_grootte = lijst->grootte;
	lijstGroei(lijst, oude_grootte + 1);
	if (lijst->grootte != oude_grootte + 1) return "Lijst groeit niet tot juiste grootte.";
	puts("[L] - LijstGroei werkt.");

	// LijstKrimp
	const int een = 1;
	lijstVoeg(lijst, &een);
	lijstKrimp(lijst);
	if (lijst->grootte != 1) return "Lijst krimpt niet goed.";
	puts("[L] - LijstKrimp werkt.");

	// Voor Elk.
	const int twee = 2;
	lijstVoeg(lijst, &twee);
	int tel = 0;
	lijstLus(lijst, i, int) { tel = tel + *i; }
	if (!tel == 3) return "Lijst lust niet goed.";
	puts("[L] - LijstLus werkt.");

	// LijstAfdrukken
	fputs("[L] - Afdrukken: ", stdout);
	lijstAfdrukken(lijst, (afdruk_opdracht)getal_afdrukker);
	putchar('\n');

	// VerwijderLijst
	verwijderde_tel = 0;
	const unsigned int verwijder_tel = lijst->tel;
	verwijderLijst(lijst, verwijder_opdracht_tel);
	if (verwijderde_tel != verwijder_tel) return "[L] - Lijst verwijdert onderdelen niet goed.";
	puts("[L] - VerwijderLijst werkt.");

	return NULL;
}

char* test_schakellijst(int argc, char** argv) { return "De Schakellijsttest is niet af."; }

char* test_sleutellijst(int argc, char** argv) { return "De Sleutellijsttest is niet af."; }


int main(int argc, char** argv) {
	if (argc < 2) return 1;
	char* fout;
	switch ((char)argv[1][0]) {
		case 'L':
			fout = test_lijst(argc, argv);
			break;
		case 's':
			fout = test_schakellijst(argc, argv);
			break;
		case 'S':
			fout = test_sleutellijst(argc, argv);
			break;
		default:
			return 1;
	}
	if (fout == NULL) return 0;
	puts(fout);
	return 1;
}
