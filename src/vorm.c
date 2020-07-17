#include "vorm.h"

#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>

Vorm* maakVorm(const Lijst* hoeken, const Lijst* vlakken) {
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	glCreateVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glCreateBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, hoeken->tel * hoeken->onderdeel_grootte, hoeken->inhoud, GL_STATIC_DRAW);

	glCreateBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vlakken->grootte * vlakken->onderdeel_grootte, vlakken->inhoud,
				 GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	Vorm* vorm = malloc(sizeof(Vorm));
	vorm->hoek_aantal = hoeken->tel;
	vorm->vlak_aantal = vlakken->tel;
	vorm->VAO = VAO;
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

void tekenVorm(const Vorm* vorm) {
	glBindVertexArray(vorm->VAO);
	glDrawElements(GL_TRIANGLES, 3 * vorm->vlak_aantal, GL_UNSIGNED_INT, 0);
}

void verwijderVorm(Vorm* vorm) {
	glDeleteVertexArrays(1, &vorm->VAO);
	free(vorm);
}
