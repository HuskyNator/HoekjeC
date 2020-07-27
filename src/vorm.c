#include "vorm.h"

#include "bestandslezer.h"
#include "materiaal.h"

#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>

Vorm* maakVorm(Lijst* hoeken, Lijst* vlakken, Lijst* groepen, Lijst* materialen) {
	// TODO verzeker lengte van groepen & materialen.
	// TODO verzeker som van groepen inhoud.

	unsigned int VAO;
	unsigned int* EBO = malloc(groepen->tel * sizeof(unsigned int));

	glCreateVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glCreateBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, hoeken->tel * sizeof(Vec3f), hoeken->inhoud, GL_STATIC_DRAW);

	glCreateBuffers(groepen->tel, EBO);
	unsigned int groep_begin = 0;
	for (unsigned int i = 0; i < groepen->tel; i++) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[i]);
		unsigned int groep_grootte = lijstKrijg(hoeken, i, unsigned int);
		Vlak* groep = &lijstKrijg(vlakken, groep_begin, Vlak);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, groep_grootte * sizeof(Vlak), groep, GL_STATIC_DRAW);
		groep_begin += groep_grootte;
	}

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);

	Vorm* vorm = malloc(sizeof(Vorm));
	vorm->hoeken = hoeken;
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
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vorm->groep_EBOs[i]);
		zetVerverMateriaal(verver, &lijstKrijg(vorm->materialen, i, Materiaal));
		glDrawElements(GL_TRIANGLES, 3 * lijstKrijg(vorm->groepen, i, unsigned int), GL_UNSIGNED_INT, 0);
	}
}

void verwijderVorm(Vorm* vorm) {
	verwijderLijst(hoeken, onwaar);
	verwijderLijst(vlakken, onwaar);
	verwijderLijst(groepen, onwaar);
	
	glDeleteVertexArrays(1, &vorm->VAO);
	glDeleteBuffers(1, vorm->groep_EBOs);
	free(vorm->groep_EBOs);
	
	free(vorm);
}
