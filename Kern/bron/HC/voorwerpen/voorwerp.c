#include "voorwerp.h"

#include "koppeling.h"
#include "lijsten/lijst.h"
#include "wiskunde/lineair.h"

#include <stdlib.h>

static void teken_opdracht_plaatsvervanger(Voorwerp* voorwerp, Verver verver){};

static struct voorwerp_opdrachten opdracht_plaatsvervanger = {.teken_opdracht = teken_opdracht_plaatsvervanger,
															  .verwijder_opdracht = verwijderVoorwerp};

Voorwerp* maakVoorwerp() {
	Voorwerp* voorwerp = malloc(sizeof(Voorwerp));
	*voorwerp = (Voorwerp){.opdrachten = &opdracht_plaatsvervanger,
						   .plek = {0, 0, 0},
						   .grootte = {1, 1, 1},
						   .draaiing = {0, 0, 0},
						   .voorwerpM = identiteitsMatrix,
						   .bijgewerkt = onwaar,
						   .ouder = NULL};
	voorwerp->verzamelM = &voorwerp->voorwerpM;
	return voorwerp;
}

void maak_voorwerp_bruikbaar(unsigned int* VAO, unsigned int* VBOs, unsigned int* EBO, const Vec3f* plekken, const Vec3f* normalen,
							 const Vec2f* verfplekken, const unsigned int* tekentallen, unsigned int hoek_aantal,
							 unsigned int tekental_aantal) {
	glGenVertexArrays(1, VAO);
	glGenBuffers(3, VBOs);
	glGenBuffers(1, EBO);

	glBindVertexArray(*VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, hoek_aantal * 3 * sizeof(float), plekken, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, onwaar, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, hoek_aantal * 3 * sizeof(float), normalen, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, onwaar, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
	glBufferData(GL_ARRAY_BUFFER, hoek_aantal * 2 * sizeof(float), verfplekken, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, onwaar, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, tekental_aantal * sizeof(unsigned int), tekentallen, GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void voorwerpZetM(Voorwerp* vorm) {
	Mat4f draai_x = draaiMatrixx(vorm->draaiing.x);
	Mat4f draai_y = draaiMatrixy(vorm->draaiing.y);
	Mat4f draai_z = draaiMatrixy(vorm->draaiing.z);
	Mat4f plek_grootte = voorwerpMatrixPlekGrootte(vorm->plek, vorm->grootte);
	vorm->voorwerpM = Mat4fMat4f(plek_grootte, Mat4fMat4f(draai_x, Mat4fMat4f(draai_y, draai_z)));
}

void voorwerpTeken(Voorwerp* voorwerp, Verver verver) {
	if (voorwerp->bijgewerkt) voorwerpZetM(voorwerp);
	if (voorwerp->ouder != NULL && (voorwerp->bijgewerkt || voorwerp->ouder->bijgewerkt)) {
		*voorwerp->verzamelM = Mat4fMat4f(*voorwerp->ouder->verzamelM, voorwerp->voorwerpM);
	}
	zetVerverMat4f(verver, "voorwerp_matrix", voorwerp->verzamelM);
	voorwerp->opdrachten->teken_opdracht(voorwerp, verver);
	// if (voorwerp->kinderen != NULL) {
	// 	for (unsigned int i = 0; i < voorwerp->kinderen->tel; i++) {
	// 		Voorwerp* kind = lijstKrijg(voorwerp->kinderen, i, Voorwerp*);
	// 		kind->opdrachten->teken_opdracht(kind, verver);
	// 	}
	// }
	voorwerp->bijgewerkt = onwaar;
}

void voorwerpZetPlek(Voorwerp* voorwerp, Vec3f plek) {
	voorwerp->plek = plek;
	voorwerp->bijgewerkt = waar;
}

void voorwerpZetGrootte(Voorwerp* voorwerp, Vec3f grootte) {
	voorwerp->grootte = grootte;
	voorwerp->bijgewerkt = waar;
}

void voorwerpZetDraai(Voorwerp* voorwerp, Vec3f draai) {
	voorwerp->draaiing = draai;
	voorwerp->bijgewerkt = waar;
}

// void voorwerpVoegF(Voorwerp* voorwerp, const Lijst* inhoud, unsigned int standplek) {
// 	unsigned int VBO;
// 	glCreateBuffers(1, &VBO);
// 	glBindBuffer(GL_ARRAY_BUFFER, VBO);
// 	glBufferData(GL_ARRAY_BUFFER, inhoud->tel * inhoud->onderdeel_grootte, inhoud->inhoud, GL_STATIC_DRAW);

// 	unsigned int getal_aantal = inhoud->onderdeel_grootte / sizeof(float);

// 	glBindVertexArray(voorwerp->VAO);
// 	glVertexAttribPointer(standplek, getal_aantal, GL_FLOAT, GL_FALSE, inhoud->onderdeel_grootte, (void*)0);
// 	glEnableVertexAttribArray(standplek);

// 	glBindVertexArray(0);
// 	glDeleteBuffers(1, &VBO);
// }

/**
 * @short Verwijdert voorwerpen & hun gegevens.
 *
 * @todo voeg kind & ouder stappen toe.
 */
void verwijderVoorwerp(Voorwerp* voorwerp) {
	voorwerp->opdrachten->verwijder_opdracht(voorwerp);
	if (voorwerp->verzamelM != &voorwerp->voorwerpM) free(voorwerp->verzamelM);
	if (voorwerp->ouder != NULL) lijstVindVerwijder(voorwerp->ouder->groepkinderen, voorwerp);
	free(voorwerp);
}
