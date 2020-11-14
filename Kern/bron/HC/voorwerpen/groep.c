#include "groep.h"

#include "lijsten/lijst.h"
#include "voorwerp.h"

#include <string.h>

void groep_teken_opdracht(Groep* groep, Verver verver) {
	for (unsigned int i = 0; i < groep->groepkinderen->tel; i++) {
		Voorwerp* kind = *lijstKrijg(groep->groepkinderen, i, Voorwerp*);
		voorwerpTeken(kind, verver);
	}
}

void groep_verwijder_opdracht(Groep* groep) {
	for (unsigned int i = 0; i < groep->groepkinderen->tel; i++) {
		Voorwerp* kind = *lijstKrijg(groep->groepkinderen, i, Voorwerp*);
		kind->ouder = NULL;	 // Voorkomen dat hij zichzelf overbodig uit de groep verwijdert.
		if (groep->VAO) {	 // Zie maakGroep.
			verwijderVoorwerp(kind);
		}
	}
	verwijderLijst(groep->groepkinderen, onwaar);
	verwijderVoorwerp(groep);
}

Groep* maakGroep(booleaan verwijder_kinderen) {
	Groep* groep = maakVoorwerp();
	groep->opdrachten = &groep_opdrachten;
	groep->groepkinderen = maakLijst(5, sizeof(Voorwerp*));

	groep->VAO = verwijder_kinderen;  // Hergebruik van veld als booleaan.

	return groep;
}

void groepVoeg(Groep* groep, Voorwerp* voorwerp) {
	voorwerp->ouder = groep;  // TODO wat als hij al in een groep zit?
	voorwerp->verzamelM = malloc(sizeof(Mat4f));
	*voorwerp->verzamelM = Mat4fMat4f(*groep->verzamelM, voorwerp->voorwerpM);
	lijstVoeg(groep->groepkinderen, &voorwerp);
}

void groepVerwijder(Groep* groep, Voorwerp* voorwerp) {
	// Voorkomt dat hij zich uit zijn ouder verwijdert.
	voorwerp->ouder = NULL;	 // TODO wat als hij in een andere groep zit?
	lijstVindVerwijder(groep->groepkinderen, voorwerp, geheugen_vergelijker, NULL);
	free(voorwerp->verzamelM);
	voorwerp->verzamelM = &voorwerp->voorwerpM;
}

// TODO wat als je zijn plek wilt behouden nadat je het verwijdert?
