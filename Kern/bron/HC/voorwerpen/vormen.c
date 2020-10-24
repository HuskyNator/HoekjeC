#include "vormen.h"

#include "GL/glew.h"
#include "booleaan.h"
#include "kleuren.h"
#include "voowerp_gegevens.h"
#include "wiskunde/lineair.h"

#include <string.h>

/**		Algemeen		**/

static const Voorwerp voorbeeld_vorm = {
	NULL, NULL, (Vec3f){0, 0, 0}, (Vec3f){1, 1, 1}, (Vec3f){0, 0, 0}, identiteitsMatrix};

static const VormGegevens voorbeeld_vormgegevens = {.soort = VORMSOORT_KLEUR, .kleur = &Wit};

void vormZetKleur(Voorwerp* voorwerp, const Vec4f* kleur) { voorwerp->vormgegevens->kleur = kleur; }

static Vorm* maakVorm(const struct voorwerp_opdrachten* opdrachten) {
	Voorwerp* vorm = malloc(sizeof(Voorwerp));
	memcpy(vorm, &voorbeeld_vorm, sizeof(Voorwerp));

	vorm->gegevens = malloc(sizeof(VormGegevens));
	memcpy(vorm->gegevens, &voorbeeld_vormgegevens, sizeof(VormGegevens));

	vorm->opdrachten = opdrachten;
	return vorm;
}

static void verwijderVorm(Vorm* vorm) {
	free(vorm->gegevens);
	free(vorm);
}

static void vorm_opzet(unsigned int* VAO, unsigned int* VBOs, unsigned int* EBO, const float* plekken,
					   const float* normalen, const float* verfplekken, const unsigned int* tallen,
					   unsigned int hoektal, unsigned int vlaktal) {
	glGenVertexArrays(1, VAO);
	glGenBuffers(3, VBOs);
	glGenBuffers(1, EBO);

	glBindVertexArray(*VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * hoektal * sizeof(float), plekken, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, onwaar, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, 3 * hoektal * sizeof(float), normalen, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, onwaar, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
	glBufferData(GL_ARRAY_BUFFER, 2 * hoektal * sizeof(float), verfplekken, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, onwaar, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * vlaktal * sizeof(unsigned int), tallen, GL_STATIC_DRAW);

	glBindVertexArray(0);
}

/**		Driehoek		**/
// TODO

/**		Vlak		**/

static void tekenVlak(Vlak* vlak, Verver verver);

static const struct voorwerp_opdrachten vlak_opdrachten = {tekenVlak, verwijderVorm};
static const float vlak_plekken[] = {-1, -1, 0, 1, -1, 0, 1, 1, 0, -1, 1, 0};
static const float vlak_normalen[] = {0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1};
static const float vlak_verfplekken[] = {0, 0, 1, 0, 1, 1, 1, 0};
static const unsigned int vlak_tallen[] = {0, 1, 2, 0, 2, 3};
static unsigned int Vlak_VAO;
static unsigned int Vlak_VBOs[3];
static unsigned int Vlak_EBO;
static booleaan vlak_opgezet = onwaar;

static void tekenVlak(Vlak* vlak, Verver verver) {	// TODO algemeen vormen
	// TODO vormsoort
	zetVerverFloat4v(verver, "kleur", (float*)vlak->vormgegevens->kleur);
	glBindVertexArray(Vlak_VAO);
	glDrawElements(GL_TRIANGLES, sizeof(vlak_tallen), GL_UNSIGNED_INT, 0);
}

Vlak* maakVlak() {
	if (!vlak_opgezet) {
		vorm_opzet(&Vlak_VAO, Vlak_VBOs, &Vlak_EBO, vlak_plekken, vlak_normalen, vlak_verfplekken,
				   vlak_tallen, sizeof(vlak_plekken) / 3, sizeof(vlak_tallen) / 3);
		vlak_opgezet = waar;
	}
	return maakVorm(&vlak_opdrachten);
}

/**		Blok		**/

static void tekenBlok(Blok* blok, Verver verver);

static const struct voorwerp_opdrachten blok_opdrachten = {tekenBlok, verwijderVorm};
static const float blok_plekken[] = {-0.5, -0.5, -0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, -0.5,
									 -0.5, -0.5, 0.5,  0.5, -0.5, 0.5,	0.5, 0.5, 0.5,	-0.5, 0.5, 0.5};
static const float blok_normalen[] = {0,  -1, 0, 1, 0,	0, 0, 0, -1, 0, 1, 0,
									  -1, 0,  0, 0, -1, 0, 0, 1, 0,	 0, 0, 1};
static const float blok_verfplekken[] = {0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0};
static const unsigned int blok_tallen[] = {3, 0, 2, 0, 1, 2, 6, 2, 1, 5, 6, 1, 6, 5, 7, 5, 4, 7,
										   3, 7, 4, 0, 3, 4, 2, 7, 3, 7, 2, 6, 4, 1, 0, 1, 4, 5};
static unsigned int blok_VAO;
static unsigned int blok_VBOs[3];
static unsigned int blok_EBO;
static booleaan blok_opgezet = onwaar;

static void tekenBlok(Blok* blok, Verver verver) {	// TODO algemeen vormen
	// TODO vormsoort
	zetVerverFloat4v(verver, "kleur", (float*)blok->vormgegevens->kleur);
	glBindVertexArray(blok_VAO);
	glDrawElements(GL_TRIANGLES, sizeof(blok_tallen), GL_UNSIGNED_INT, 0);
}

Vlak* maakBlok() {
	if (!blok_opgezet) {
		vorm_opzet(&blok_VAO, blok_VBOs, &blok_EBO, blok_plekken, blok_normalen, blok_verfplekken,
				   blok_tallen, sizeof(blok_plekken) / 3, sizeof(blok_tallen) / 3);
		blok_opgezet = waar;
	}
	return maakVorm(&blok_opdrachten);
}