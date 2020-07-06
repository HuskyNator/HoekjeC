#include "vorm.h"

#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>

struct vorm {
	unsigned int grootte;
	unsigned int VAO;
};

Vorm* maakVorm(const Vec3f* hoeken, const size_t hoekengrootte, const Vec3ui* hoektallen,
			   const size_t hoektallengrootte) {
	unsigned int grootte = hoektallengrootte / sizeof(unsigned int);
	unsigned int VAO;
	unsigned int EBO;

	glCreateVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glCreateBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, hoektallengrootte, hoektallen, GL_STATIC_DRAW);

	unsigned int VBO;

	glCreateBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, hoekengrootte, hoeken, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	Vorm* vorm = malloc(sizeof(vorm));
	vorm->VAO = VAO;
	vorm->grootte = grootte;
	return vorm;
}

void vormVoegInhoudToe(Vorm* vorm, const float inhoud[], size_t inhoudsgrootte, unsigned int onderdeelgrootte,
					   unsigned int standplaats) {
	unsigned int VBO;
	glCreateBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, inhoudsgrootte, inhoud, GL_STATIC_DRAW);

	glBindVertexArray(vorm->VAO);
	glVertexAttribPointer(standplaats, onderdeelgrootte, GL_FLOAT, GL_FALSE, onderdeelgrootte * sizeof(float),
						  (void*)0);
	glEnableVertexAttribArray(standplaats);

	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
}

void tekenVorm(const Vorm* vorm) {
	glBindVertexArray(vorm->VAO);
	glDrawElements(GL_TRIANGLES, vorm->grootte, GL_UNSIGNED_INT, 0);
}

void verwijderVorm(Vorm* vorm) {
	glDeleteVertexArrays(1, &vorm->VAO);
	free(vorm);
}
