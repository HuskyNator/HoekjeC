#include "vormen.h"

#include "GL/glew.h"
#include "booleaan.h"
#include "kleuren.h"
#include "voowerp_gegevens.h"
#include "wiskunde/lineair.h"

#include <string.h>

/**		Algemeen		**/

void vormZetKleur(Voorwerp* voorwerp, const Vec4f* kleur) { voorwerp->vormgegevens->kleur = kleur; }

static void tekenVorm(Vorm* vorm, Verver verver) {
	zetVerverFloat4v(verver, "kleur", (float*)vorm->vormgegevens->kleur);
	glBindVertexArray(vorm->VAO);
	glDrawElements(GL_TRIANGLES, vorm->EBO_tal, GL_UNSIGNED_INT, 0);
}

static void verwijderVorm(Vorm* vorm) {
	free(vorm->gegevens);
	free(vorm);
}

static const struct voorwerp_opdrachten vorm_opdrachten = {tekenVorm, verwijderVorm};

static Vorm* maakVorm(unsigned int VAO, unsigned int EBO_tal) {
	Voorwerp* vorm = malloc(sizeof(Voorwerp));
	*vorm = (Voorwerp){.opdrachten = &vorm_opdrachten,
					   .VAO = VAO,
					   .EBO_tal = EBO_tal,
					   .plek = {0, 0, 0},
					   .grootte = {1, 1, 1},
					   .draaiing = {0, 0, 0},
					   .voorwerpM = identiteitsMatrix,
					   .bijgewerkt = onwaar};

	vorm->vormgegevens = malloc(sizeof(VormGegevens));
	*vorm->vormgegevens = (VormGegevens){.soort = VORMSOORT_KLEUR, .kleur = &Wit};
	return vorm;
}

static void vorm_opzet(unsigned int* VAO, unsigned int* VBOs, unsigned int* EBO, const float* plekken, const float* normalen,
					   const float* verfplekken, const unsigned int* tallen, unsigned int hoektal, unsigned int verftal,
					   unsigned int vlaktal) {
	glGenVertexArrays(1, VAO);
	glGenBuffers(3, VBOs);
	glGenBuffers(1, EBO);

	glBindVertexArray(*VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, hoektal * sizeof(float), plekken, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, onwaar, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, hoektal * sizeof(float), normalen, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, onwaar, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
	glBufferData(GL_ARRAY_BUFFER, verftal * sizeof(float), verfplekken, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, onwaar, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vlaktal * sizeof(unsigned int), tallen, GL_STATIC_DRAW);

	glBindVertexArray(0);
}

/**		Driehoek		**/
static const float Driehoek_plekken[] = {0, 0, 0, 1, 0, 0, 0.5, 1, 0};
static const float Driehoek_normalen[] = {0, 0, -1, 0, 0, -1, 0, 0, -1};
static const float Driehoek_verfplekken[] = {0, 0, 1, 0, 0.5, 1};
static const unsigned int Driehoek_tallen[] = {0, 1, 2};
static unsigned int Driehoek_VAO;
static unsigned int Driehoek_VBOs[3];
static unsigned int Driehoek_EBO;
static booleaan Driehoek_opgezet = onwaar;

Vlak* maakDriehoek() {
	if (!Driehoek_opgezet) {
		vorm_opzet(&Driehoek_VAO, Driehoek_VBOs, &Driehoek_EBO, Driehoek_plekken, Driehoek_normalen, Driehoek_verfplekken, Driehoek_tallen,
				   sizeof(Driehoek_plekken), sizeof(Driehoek_verfplekken), sizeof(Driehoek_tallen));
		Driehoek_opgezet = waar;
	}
	return maakVorm(Driehoek_VAO, sizeof(Driehoek_tallen));
}

/**		Vlak		**/

static const float Vlak_plekken[] = {-1, -1, 0, 1, -1, 0, 1, 1, 0, -1, 1, 0};
static const float Vlak_normalen[] = {0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1};
static const float Vlak_verfplekken[] = {0, 0, 1, 0, 1, 1, 1, 0};
static const unsigned int Vlak_tallen[] = {0, 1, 2, 0, 2, 3};
static unsigned int Vlak_VAO;
static unsigned int Vlak_VBOs[3];
static unsigned int Vlak_EBO;
static booleaan Vlak_opgezet = onwaar;

Vlak* maakVlak() {
	if (!Vlak_opgezet) {
		vorm_opzet(&Vlak_VAO, Vlak_VBOs, &Vlak_EBO, Vlak_plekken, Vlak_normalen, Vlak_verfplekken, Vlak_tallen, sizeof(Vlak_plekken),
				   sizeof(Vlak_verfplekken), sizeof(Vlak_tallen));
		Vlak_opgezet = waar;
	}
	return maakVorm(Vlak_VAO, sizeof(Vlak_tallen));
}

/**		Blok		**/

static const float Blok_plekken[] = {-0.5, -0.5, -0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, -0.5,
									 -0.5, -0.5, 0.5,  0.5, -0.5, 0.5,	0.5, 0.5, 0.5,	-0.5, 0.5, 0.5};
static const float Blok_normalen[] = {0, -1, 0, 1, 0, 0, 0, 0, -1, 0, 1, 0, -1, 0, 0, 0, -1, 0, 0, 1, 0, 0, 0, 1};
static const float Blok_verfplekken[] = {0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0};
static const unsigned int Blok_tallen[] = {3, 0, 2, 0, 1, 2, 6, 2, 1, 5, 6, 1, 6, 5, 7, 5, 4, 7,
										   3, 7, 4, 0, 3, 4, 2, 7, 3, 7, 2, 6, 4, 1, 0, 1, 4, 5};
static unsigned int Blok_VAO;
static unsigned int Blok_VBOs[3];
static unsigned int Blok_EBO;
static booleaan Blok_opgezet = onwaar;

Vlak* maakBlok() {
	if (!Blok_opgezet) {
		vorm_opzet(&Blok_VAO, Blok_VBOs, &Blok_EBO, Blok_plekken, Blok_normalen, Blok_verfplekken, Blok_tallen, sizeof(Blok_plekken),
				   sizeof(Blok_verfplekken), sizeof(Blok_tallen));
		Blok_opgezet = waar;
	}
	return maakVorm(Blok_VAO, sizeof(Blok_tallen));
}
