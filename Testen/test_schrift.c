#include "HC/schrift.h"

#include <stdio.h>

char* test(int argc, char const* argv[]) {
	/**		MAAK		**/
	// MaakSchrift
	Schrift* schrift_leeg = maakSchrift(NULL);	// Leeg
	if (schrift_leeg->onderdeel_grootte != sizeof(char) || schrift_leeg->tel > 0) return ("Kon schrift niet goed maken. (1)");

	const char* proef = "Proef";  // Met inhoud
	Schrift* schrift_proef = maakSchrift(proef);
	if (schrift_proef->tel != 5) return ("Kon schrift niet goed maken. (2)");
	for (unsigned int i = 0; i < 5; i++) {
		if (((char*)schrift_proef->inhoud)[i] != proef[i]) return "Kon schrift niet goed maken. (3)";
	}
	puts("Schrift - maakSchrift werkt.");

	// LeesSchrift
	FILE* bestand = fopen("hulpbronnen/test_schrift.txt", "r");
	if (bestand == NULL) return "Kon test_schrift.text niet openen.";
	Schrift* bestand_s = leesSchrift(bestand);
	fclose(bestand);
	const char* appelmoes = "Appel\n\tMoes.";

	printf("Schrift - Lezen:\n\tVerwacht:\n\"%s\"\nGekregen:\n\"", appelmoes);
	schriftAfdrukken(bestand_s);  // Mogelijk onjuist.
	puts("\"");

	if (bestand_s->tel != 12) "Kon schrift niet goed maken. (lezen 1)";
	for (unsigned int i = 0; i < 12; i++) {
		if (((char*)bestand_s->inhoud)[i] != appelmoes[i]) return "Kon schrift niet goed maken. (lezen 2)";
	}
	puts("Schrift - leesSchrift werkt.");

	// MaakSchrift_const
	const Schrift const_schrift = maakSchrift_const("Const.");
	const char* const_inhoud = "Const.";
	if (const_schrift.grootte != 7 || const_schrift.tel != 6) return "Kon schrift niet goed maken. (const 1)";
	for (unsigned int i = 0; i <= const_schrift.tel; i++) {
		if (((char*)const_schrift.inhoud)[i] != const_inhoud[i]) return "Kon schrift niet goed maken. (const 2).";
	}
	puts("Schrift - maakSchrift_const werkt.");

	/**		GEBRUIK		**/
	// SchriftVoegVoor
	Schrift* voeg_voor = maakSchrift("a");
	schriftVoegVoor(voeg_voor, 'b');
	if (voeg_voor->tel != 2 || ((char*)voeg_voor->inhoud)[0] != 'b' || ((char*)voeg_voor->inhoud)[1] != 'a')
		return "Schrift kon niet goed voor voegen.";
	puts("Schrift- schriftVoegVoor werkt.");

	// SchriftVoegNa
	Schrift* voeg_na = maakSchrift("c");
	schriftVoegNa(voeg_na, 'd');
	if (voeg_voor->tel != 2 || ((char*)voeg_na->inhoud)[0] != 'c' || ((char*)voeg_na->inhoud)[1] != 'd')
		return "Schrift kon niet goed na voegen.";
	puts("Schrift - schriftVoegNa werkt.");

	// SchriftVoegIn
	Schrift* voeg_in = maakSchrift("123");
	schriftVoegIn(voeg_in, 0, 'a');
	schriftVoegIn(voeg_in, 2, 'b');
	schriftVoegIn(voeg_in, 4, 'c');
	schriftVoegIn(voeg_in, 6, 'd');
	const char* voeg_in_verwacht = "a1b2c3d";
	if (voeg_in->tel != 7) return "Schrift kon niet goed in voegen. (1)";
	for (unsigned int i = 0; i < 7; i++) {
		if (((char*)voeg_in->inhoud)[i] != voeg_in_verwacht[i]) return "Schrift kon niet goed in voegen. (2)";
	}
	puts("Schrift - schriftVoegIn werkt.");

	// SchriftVoegVoor_s
	Schrift* voeg_voor_s = maakSchrift("0");
	const char* voeg_voor_s_voegsel = "123";
	schriftVoegVoorMeer(voeg_voor_s, voeg_voor_s_voegsel, 0);
	if (voeg_voor_s->tel != 4) return "Schrift kon niet goed meervoudig voor voegen. (1)";

	const char* voeg_voor_s_inhoud = "121230";
	schriftVoegVoorMeer(voeg_voor_s, voeg_voor_s_voegsel, 2);
	if (voeg_voor_s->tel != 6) return "Schrift kon niet goed meervoudig voor voegen. (2)";
	for (unsigned int i = 0; i < 6; i++) {
		if (((char*)voeg_voor_s->inhoud)[i] != voeg_voor_s_inhoud[i]) return "Schrift kon niet goed meervoudig voor voegen. (3)";
	}
	puts("Schrift - schriftVoegVoorMeer werkt.");

	// SchriftVoegNa_s
	Schrift* voeg_na_s = maakSchrift("0");
	const char* voeg_na_s_voegsel = "123";
	schriftVoegNaMeer(voeg_na_s, voeg_na_s_voegsel, 0);
	if (voeg_na_s->tel != 4) return "Schrift kon niet goed meervoudig na voegen. (1)";

	const char* voeg_na_s_inhoud = "012312";
	schriftVoegNaMeer(voeg_na_s, voeg_na_s_voegsel, 2);
	if (voeg_na_s->tel != 6) return "Schrift kon niet goed meervoudig na voegen. (2)";
	for (unsigned int i = 0; i < 3; i++) {
		if (((char*)voeg_na_s->inhoud)[i] != voeg_na_s_inhoud[i]) return "Schrift kon niet goed meervoudig na voegen. (3)";
	}
	puts("Schrift - schriftVoegNaMeer werkt.");

	// SchriftVoegIn_s
	Schrift* voeg_in_s = maakSchrift("0e");
	const char* voeg_in_s_voegsel = "123";
	schriftVoegInMeer(voeg_in_s, 0, voeg_in_s_voegsel, 0);
	if (voeg_in_s->tel != 5) return "Schrift kon niet goed meervoudig in voegen. (1)";

	schriftVoegInMeer(voeg_in_s, 4, voeg_in_s_voegsel, 1);  // Buiten schrift.
	if (voeg_in_s->tel != 6) return "Schrift kon niet goed meervoudig in voegen. (2)";

	const char* voeg_in_s_inhoud = "aa1b2ddd301e";
	const char* voeg_in_s_voegsel1 = "aaaa";
	const char* voeg_in_s_voegsel2 = "bbbbb";
	const char* voeg_in_s_voegsel3 = "ddddddddddddd";
	schriftVoegInMeer(voeg_in_s, 0, voeg_in_s_voegsel1, 2);
	schriftVoegInMeer(voeg_in_s, 3, voeg_in_s_voegsel2, 1);
	schriftVoegInMeer(voeg_in_s, 5, voeg_in_s_voegsel3, 3);
	if (voeg_in_s->tel != 12) return "Schrift kon niet goed meervoudig in voegen. (3)";
	for (unsigned int i = 0; i < sizeof(voeg_in_s_inhoud); i++) {
		if (((char*)voeg_in_s->inhoud)[i] != voeg_in_s_inhoud[i]) return "Schrift kon niet goed meervoudig in voegen. (4)";
	}
	puts("Schrift - schriftVoegInMeer werkt.");

	// SchriftKrimp
	Schrift* krimp = maakSchrift(NULL);
	unsigned int krimptel = 0;
	while (krimp->grootte == krimp->tel) {
		krimptel++;
		if (krimptel == 20) return "Kan schriftkrimpen niet beproeven.";
		schriftVoegNa(krimp, 'a');
	}
	if (krimp->grootte <= krimp->tel) "Grootte van schrift is te klein voor inhoud.";
	const unsigned int vorige_tel = krimp->tel;
	schriftKrimp(krimp);
	if (krimp->grootte != vorige_tel || krimp->tel != vorige_tel) return "Schrift kon niet goed krimpen.";
	puts("Schrift - schriftKrimp werkt.");

	// SchriftKrijg
	Schrift* krijg = maakSchrift("abc");
	if (schriftKrijg(krijg, 0) != 'a') return "Schrift krijgt niet goed. (1)";
	if (schriftKrijg(krijg, 1) != 'b') return "Schrift krijgt niet goed. (2)";
	if (schriftKrijg(krijg, 2) != 'c') return "Schrift krijgt niet goed. (3)";
	puts("Schrift - schriftKrijg werkt.");

	// SchriftVervang
	Schrift* vervang = maakSchrift("a2a2");
	schriftVervangChar(vervang, '2', 'c');
	if (schriftKrijg(vervang, 0) != 'a') return "Schrift vervangt niet goed. (1)";
	if (schriftKrijg(vervang, 1) != 'c') return "Schrift vervangt niet goed. (2)";
	if (schriftKrijg(vervang, 2) != 'a') return "Schrift vervangt niet goed. (3)";
	if (schriftKrijg(vervang, 3) != 'c') return "Schrift vervangt niet goed. (4)";
	puts("Schrift - schriftVervangChar werkt.");

	// SchriftGelijk (& SchriftVergelijker)
	Schrift* gelijk1 = maakSchrift("abc12");
	Schrift* gelijk2 = maakSchrift("abc123");
	Schrift* gelijk3 = maakSchrift(NULL);
	Schrift* gelijk4 = maakSchrift(NULL);

	if (schriftGelijk(gelijk1, gelijk2)) return "Schrift vergelijkt niet goed. (1)";
	schriftVoegNa(gelijk1, '3');
	if (!schriftGelijk(gelijk1, gelijk2)) return "Schrift vergelijkt niet goed. (2)";
	if (!schriftGelijk(gelijk1, gelijk2)) return "Schrift vergelijkt niet goed. (3)";
	puts("Schrift - schriftGelijk werkt.");

	// SchriftDeelGelijk
	Schrift* deelgelijk1 = maakSchrift("4ab2");
	Schrift* deelgelijk2 = maakSchrift("ab");
	if (!schriftDeelGelijk(deelgelijk1, 1, deelgelijk2)) return "Schrift vergelijkt deel niet goed.";

	// SchriftSleutel
	// Algemeen eigenaardig maar mogelijk gelijk.
	Schrift* sleutel1 = maakSchrift(NULL);
	Schrift* sleutel2 = maakSchrift(NULL);
	Schrift* sleutel3 = maakSchrift("ab");
	Schrift* sleutel4 = maakSchrift("a.");
	Schrift* sleutel5 = maakSchrift("a");
	unsigned int sleutel1sleutel = schriftSleutel(sleutel1);
	unsigned int sleutel2sleutel = schriftSleutel(sleutel2);
	unsigned int sleutel3sleutel = schriftSleutel(sleutel3);
	unsigned int sleutel4sleutel = schriftSleutel(sleutel4);
	unsigned int sleutel5sleutel = schriftSleutel(sleutel5);

	if (sleutel1sleutel != sleutel2sleutel) return "Schrift sleutelt niet goed. (1)";
	if (sleutel1sleutel == sleutel3sleutel) return "Schrift sleutelt niet goed. (2)";
	if (sleutel1sleutel == sleutel4sleutel) return "Schrift sleutelt niet goed. (3)";
	if (sleutel1sleutel == sleutel5sleutel) return "Schrift sleutelt niet goed. (4)";

	if (sleutel3sleutel == sleutel4sleutel) return "Schrift sleutelt niet goed. (5)";
	if (sleutel3sleutel == sleutel5sleutel) return "Schrift sleutelt niet goed. (6)";

	if (sleutel4sleutel == sleutel5sleutel) return "Schrift sleutelt niet goed. (7)";

	schriftVoegNa(sleutel5, 'b');
	sleutel5sleutel = schriftSleutel(sleutel5);
	if (sleutel3sleutel != sleutel5sleutel) return "Schrift sleutelt niet goed. (8)";
	puts("Schrift - schriftSleutel werkt.");

	// SchriftAfdrukken
	puts("Schrift - Afdrukken: ");
	const Schrift afdrukken = maakSchrift_const("Werkt.");
	schriftAfdrukken(&afdrukken);
	putchar('\n');

	/**		VERWIJDER		**/
	// VerwijderSchrift
	Schrift* verwijder = maakSchrift("abc");
	verwijderSchrift(verwijder);
	puts("Schrift - verwijderSchrift werkt hoogstwaarschijnlijk.");

	/**		VERANDER		**/
	Schrift* naarchar = maakSchrift("verander proefstelling");
	const char* verwachttechar = "verander proefstelling";
	char* gekregenchar = schriftNaarChar(naarchar);
	for (unsigned int i = 0; i < sizeof(verwachttechar); i++) {
		if (verwachttechar[i] != gekregenchar[i]) return "Schrift verandert niet goed naar char.";
	}
	puts("Schrift - schriftNaarChar werkt hoogstwaarschijnlijk.");

	return NULL;
}

int main(int argc, char const* argv[]) {
	char* fout = test(argc, argv);
	if (fout == NULL) return 0;
	puts(fout);
	return 1;
}
