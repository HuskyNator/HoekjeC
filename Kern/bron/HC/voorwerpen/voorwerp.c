#include "voorwerp.h"

#include "koppeling.h"

void voorwerpZetM(Voorwerp* vorm) {
	Mat4f draai_x = draaiMatrixx(vorm->draaiing.x);
	Mat4f draai_y = draaiMatrixy(vorm->draaiing.y);
	Mat4f draai_z = draaiMatrixy(vorm->draaiing.z);
	Mat4f plek_grootte = voorwerpMatrixPlekGrootte(vorm->plek, vorm->grootte);
	vorm->voorwerpM = Mat4fMat4f(plek_grootte, Mat4fMat4f(draai_x, Mat4fMat4f(draai_y, draai_z)));
}

void voorwerpTeken(Voorwerp* voorwerp, Verver verver) {
	if (voorwerp->bijgewerkt) {
		voorwerpZetM(voorwerp);
		voorwerp->bijgewerkt = onwaar;
	}
	zetVerverMat4f(verver, "voorwerp_matrix", &voorwerp->voorwerpM);
	voorwerp->opdrachten->teken_opdracht(voorwerp, verver);
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

#define verwijderVoorwerp(voorwerp) voorwerp->opdrachten->verwijder_opdracht(voorwerp)
