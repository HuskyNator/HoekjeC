#include "vorm.h"

#include "bestandslezer.h"
#include "materiaal.h"

#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>

Vorm* maakVorm(Lijst* hoeken_p, Lijst* hoeken_v, Lijst* hoeken_n, Lijst* vlakken, Lijst* groepen, Lijst* materialen) {
	if (hoeken_p->tel != hoeken_v->tel || hoeken_p->tel != hoeken_n->tel) {
		fputs("Vormen kunnen enkel gemaakt worden met hoeklijsten van dezelfde grootte (p/v/n).\n", stderr);
		return NULL;
	}

	unsigned int VAO;
	unsigned int* EBO = malloc(groepen->tel * sizeof(unsigned int));

	glCreateVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO_p;
	glCreateBuffers(1, &VBO_p);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_p);
	glBufferData(GL_ARRAY_BUFFER, hoeken_p->tel * sizeof(Vec3f), hoeken_p->inhoud, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3f), (void*)0);
	glEnableVertexAttribArray(0);

	unsigned int VBO_v;
	glCreateBuffers(1, &VBO_v);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_v);
	glBufferData(GL_ARRAY_BUFFER, hoeken_v->tel * sizeof(Vec2f), hoeken_v->inhoud, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vec2f), (void*)0);
	glEnableVertexAttribArray(1);

	unsigned int VBO_n;
	glCreateBuffers(1, &VBO_n);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_n);
	glBufferData(GL_ARRAY_BUFFER, hoeken_n->tel * sizeof(Vec3f), hoeken_n->inhoud, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3f), (void*)0);
	glEnableVertexAttribArray(2);

	glCreateBuffers(groepen->tel, EBO);
	unsigned int begin_tel = 0;
	for (unsigned int i = 0; i < groepen->tel; i++) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[i]);
		VlakGroep* groep = &lijstKrijg(groepen, i, VlakGroep);
		Vlak* begin = &lijstKrijg(vlakken, begin_tel, Vlak);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, groep->grootte * sizeof(Vlak), begin, GL_STATIC_DRAW);
		begin_tel += groep->grootte;

		if (groep->materiaal_tal >= materialen->tel) {
			fputs("Vorm is mogelijk onjuist! Een gegeven groep verwijst naar een ontbrekend materiaal.\n", stderr);
		}
	}

	if (begin_tel != vlakken->tel) {
		fprintf(stderr, "Vorm is mogelijk onjuist! De gegeven groepen gebruiken %u van de %u vlakken.\n", begin_tel,
				vlakken->tel);
	}

	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO_p);
	glDeleteBuffers(1, &VBO_v);
	glDeleteBuffers(1, &VBO_n);

	Vorm* vorm = malloc(sizeof(Vorm));
	vorm->hoek_aantal = hoeken_p->tel;
	vorm->hoeken_p = hoeken_p;
	vorm->hoeken_v = hoeken_v;
	vorm->hoeken_n = hoeken_n;
	vorm->vlakken = vlakken;
	vorm->groepen = groepen;
	vorm->materialen = materialen;

	vorm->VAO = VAO;
	vorm->groep_EBOs = EBO;
	return vorm;
}

void vormVoegInhoudToe(Vorm* vorm, const Lijst* inhoud, unsigned int standplaats) {
	unsigned int VBO;
	glCreateBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, inhoud->tel * inhoud->onderdeel_grootte, inhoud->inhoud, GL_STATIC_DRAW);

	unsigned int getal_aantal = inhoud->onderdeel_grootte / sizeof(float);

	glBindVertexArray(vorm->VAO);
	glVertexAttribPointer(standplaats, getal_aantal, GL_FLOAT, GL_FALSE, inhoud->onderdeel_grootte, (void*)0);
	glEnableVertexAttribArray(standplaats);

	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
}

void tekenVorm(const Vorm* vorm, Verver* verver) {
	glBindVertexArray(vorm->VAO);
	for (unsigned int i = 0; i < vorm->groepen->tel; i++) {
		VlakGroep* vg = &lijstKrijg(vorm->groepen, i, VlakGroep);
		Materiaal* materiaal = &lijstKrijg(vorm->materialen, vg->materiaal_tal, Materiaal);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vorm->groep_EBOs[i]);
		zetVerverMateriaal(verver, materiaal);
		glDrawElements(GL_TRIANGLES, 3 * vg->grootte, GL_UNSIGNED_INT, (void*)0);
	}
}

void verwijderVorm(Vorm* vorm) {
	glDeleteVertexArrays(1, &vorm->VAO);
	glDeleteBuffers(vorm->groepen->tel, vorm->groep_EBOs);
	free(vorm->groep_EBOs);

	verwijderLijst(vorm->hoeken_p, onwaar);
	verwijderLijst(vorm->hoeken_v, onwaar);
	verwijderLijst(vorm->hoeken_n, onwaar);
	verwijderLijst(vorm->vlakken, onwaar);
	verwijderLijst(vorm->groepen, onwaar);
	lijstVoorElk(vorm->materialen, (voor_elk_opdracht)verwijderMateriaal);

	free(vorm);
}
