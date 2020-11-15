#include "vormen.h"

#include "GL/glew.h"
#include "algemeen.h"
#include "kleuren.h"
#include "voowerp_gegevens.h"
#include "wiskunde/lineair.h"

#include <string.h>

/**		Algemeen		**/

void vormZetKleur(Voorwerp* voorwerp, const Vec4f* kleur) { voorwerp->vormgegevens->kleur = kleur; }

/**
 * @brief Tekent vormen.
 *
 * @param  vorm: De vorm om te tekenen.
 * @param  verver: De verver waarmee getekend wordt.
 * @retval None
 * @todo Voeg ondersteuning toe voor afbeeldingen en materialen.
 */
static void tekenVorm(Vorm* vorm, Verver verver) {
	glBindVertexArray(vorm->VAO);

	const VormGegevens gegevens = *vorm->vormgegevens;

	zetVerverUint(verver, "soort", gegevens.soort);
	if (gegevens.soort == VORMSOORT_KLEUR) {
		zetVerverFloat4v(verver, "kleur", (float*)vorm->vormgegevens->kleur);
		glDrawElements(GL_TRIANGLES, vorm->EBO_aantal, GL_UNSIGNED_INT, 0);
	}

	if (gegevens.rand) {
		zetVerverUint(verver, "soort", VORMSOORT_LIJN);
		zetVerverFloat4v(verver, "kleur", (float*)vorm->vormgegevens->rand_kleur);
		glDrawElements(GL_LINE_LOOP, vorm->EBO_aantal, GL_UNSIGNED_INT, 0);
	}
}

static void verwijderVorm_opdracht(Vorm* vorm) { free(vorm->gegevens); }

static const struct voorwerp_opdrachten vorm_opdrachten = {tekenVorm, verwijderVorm_opdracht};

static Vorm* maakVorm() {
	Voorwerp* vorm = maakVoorwerp();
	vorm->opdrachten = &vorm_opdrachten;
	vorm->vormgegevens = malloc(sizeof(VormGegevens));
	*vorm->vormgegevens = (VormGegevens){.soort = VORMSOORT_KLEUR, .rand = onwaar, .rand_kleur = &Groen, .kleur = &LichtGrijs};
	return vorm;
}

/**		Driekant		**/

static const Vec3f Driekant_plekken[] = {{0, 0, 0}, {1, 0, 0}, {0.5, 1, 0}};
static const Vec3f Driekant_normalen[] = {{0, 0, -1}, {0, 0, -1}, {0, 0, -1}};
static const Vec2f Driekant_verfplekken[] = {{0, 0}, {1, 0}, {0.5, 1}};
static const Vec3u Driekant_tallen[] = {{0, 1, 2}};
static unsigned int Driekant_VAO;
static unsigned int Driekant_VBOs[3];
static unsigned int Driekant_EBO;
static booleaan Driekant_opgezet = onwaar;

Driekant* maakKant() {
	if (!Driekant_opgezet) {
		maak_voorwerp_bruikbaar(&Driekant_VAO, Driekant_VBOs, &Driekant_EBO, Driekant_plekken, Driekant_normalen, Driekant_verfplekken,
								Driekant_tallen, sizeof(Driekant_plekken) / (3 * sizeof(float)),
								sizeof(Driekant_tallen) / (3 * sizeof(unsigned int)));
		Driekant_opgezet = waar;
	}
	Driekant* driekant = maakVorm();
	driekant->VAO = Driekant_VAO;
	driekant->VBOs[0] = Driekant_VBOs[0];
	driekant->VBOs[1] = Driekant_VBOs[1];
	driekant->VBOs[2] = Driekant_VBOs[2];
	driekant->EBO_aantal = sizeof(Driekant_tallen) / sizeof(unsigned int);
	return driekant;
}

/**		Vierkant		**/

static const Vec3f Vierkant_plekken[] = {{-1, -1, 0}, {1, -1, 0}, {1, 1, 0}, {-1, 1, 0}};
static const Vec3f Vierkant_normalen[] = {{0, 0, -1}, {0, 0, -1}, {0, 0, -1}, {0, 0, -1}};
static const Vec2f Vierkant_verfplekken[] = {{0, 0}, {1, 0}, {1, 1}, {1, 0}};
static const Vec3u Vierkant_tallen[] = {{0, 1, 2}, {0, 2, 3}};
static unsigned int Vierkant_VAO;
static unsigned int Vierkant_VBOs[3];
static unsigned int Vierkant_EBO;
static booleaan Vierkant_opgezet = onwaar;

Vierkant* maakVierkant() {
	if (!Vierkant_opgezet) {
		maak_voorwerp_bruikbaar(&Vierkant_VAO, Vierkant_VBOs, &Vierkant_EBO, Vierkant_plekken, Vierkant_normalen, Vierkant_verfplekken,
								Vierkant_tallen, sizeof(Vierkant_plekken) / (3 * sizeof(float)),
								sizeof(Vierkant_tallen) / (3 * sizeof(unsigned int)));
		Vierkant_opgezet = waar;
	}
	Vierkant* vierkant = maakVorm();
	vierkant->VAO = Vierkant_VAO;
	vierkant->VBOs[0] = Vierkant_VBOs[0];
	vierkant->VBOs[1] = Vierkant_VBOs[1];
	vierkant->VBOs[2] = Vierkant_VBOs[2];
	vierkant->EBO_aantal = sizeof(Vierkant_tallen) / sizeof(unsigned int);
	return vierkant;
}

/**		Blok		**/
// TODO: Maak blok lijnen gelijkspiegelig.
static const Vec3f Blok_plekken[] = {{-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5}, {0.5, 0.5, -0.5}, {-0.5, 0.5, -0.5},
									 {-0.5, -0.5, 0.5},	 {0.5, -0.5, 0.5},	{0.5, 0.5, 0.5},  {-0.5, 0.5, 0.5}};
static const Vec3f Blok_normalen[] = {{0, 0, -1}, {0, -1, 0}, {0, 0, -1}, {0, 1, 0}, {-1, 0, 0}, {0, 0, 1}, {1, 0, 0}, {0, 0, 1}};
static const Vec2f Blok_verfplekken[] = {{0, 0}, {1, 0}, {1, 1}, {0, 1}, {1, 1}, {0, 1}, {0, 0}, {1, 0}};
static const Vec3u Blok_tallen[] = {{3, 0, 2}, {0, 1, 2}, {2, 1, 6}, {1, 5, 6}, {6, 5, 7}, {5, 4, 7},
									{3, 7, 4}, {0, 3, 4}, {0, 4, 1}, {6, 7, 3}, {2, 6, 3}, {4, 5, 1}};
static unsigned int Blok_VAO;
static unsigned int Blok_VBOs[3];
static unsigned int Blok_EBO;
static booleaan Blok_opgezet = onwaar;

Blok* maakBlok() {
	if (!Blok_opgezet) {
		maak_voorwerp_bruikbaar(&Blok_VAO, Blok_VBOs, &Blok_EBO, Blok_plekken, Blok_normalen, Blok_verfplekken, Blok_tallen,
								sizeof(Blok_plekken) / (3 * sizeof(float)), sizeof(Blok_tallen) / (3 * sizeof(unsigned int)));
		Blok_opgezet = waar;
	}
	Blok* blok = maakVorm();
	blok->VAO = Blok_VAO;
	blok->VBOs[0] = Blok_VBOs[0];
	blok->VBOs[1] = Blok_VBOs[1];
	blok->VBOs[2] = Blok_VBOs[2];
	blok->EBO_aantal = sizeof(Blok_tallen) / sizeof(unsigned int);
	return blok;
}

/**		Driehoek		**/

Driehoek* maakDriehoek(const Vec3f* plekken, const Vec3f* normalen, const Vec2f* verfplekken) {
	Driehoek* driehoek = maakVorm();
	maak_voorwerp_bruikbaar(&driehoek->VAO, driehoek->VBOs, &driehoek->EBO, plekken, normalen, verfplekken, Driekant_tallen, 3, 1);
	return driehoek;
}

/**		Vierhoek		**/

Vierhoek* maakVierhoek(const Vec3f* plekken, const Vec3f* normalen, const Vec2f* verfplekken) {
	Vierhoek* vierhoek = maakVorm();
	maak_voorwerp_bruikbaar(&vierhoek->VAO, vierhoek->VBOs, &vierhoek->EBO, plekken, normalen, verfplekken, Vierkant_tallen, 4, 2);
	return vierhoek;
}
