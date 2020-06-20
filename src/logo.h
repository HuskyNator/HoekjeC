
#include "verver.h"
#include "vorm.h"

#include <GL/glew.h>
#include <stdlib.h>

#define SCHERM_BREEDTE (1920 / 2)
#define SCHERM_HOOGTE (1080 / 2)

static float ROOD[] = {1, 0, 0, 1};
static float BLAUW[] = {0, 0, 1, 1};
static float WIT[] = {1, 1, 1, 1};

static Vorm* rood_driehoek;
static Vorm* blauw_driehoek;
static Verver* logo_verver;

void maakLogo(/*unsigned int grootte*/) {
	logo_verver = maakVerver("./shaders/normaal.vert", "./shaders/normaal.frag");
	const float rode_hoeken[] = {-0.9 / SCHERM_BREEDTE * SCHERM_HOOGTE * 0.15 * 2, 1 * 0.015 * 2,	 -0.1,
								 -0.7 / SCHERM_BREEDTE * SCHERM_HOOGTE * 0.15 * 2, -2.5 * 0.015 * 2, -0.1,
								 1.0 / SCHERM_BREEDTE * SCHERM_HOOGTE * 0.15 * 2,  1 * 0.015 * 2,	 -0.1};

	const float blauwe_hoeken[] = {
		(-1.0 / SCHERM_BREEDTE * SCHERM_HOOGTE + 0.1) * 0.15 * 2, (1 + 0.05) * 0.15 * 2,   0,
		(-0.5 / SCHERM_BREEDTE * SCHERM_HOOGTE + 0.1) * 0.15 * 2, (-1 + 0.05) * 0.15 * 2,  0,
		(1.0 / SCHERM_BREEDTE * SCHERM_HOOGTE + 0.1) * 0.15 * 2,  (0.5 + 0.05) * 0.15 * 2, 0};

	const unsigned int hoektallen[] = {0, 1, 2};

	rood_driehoek = maakVorm(rode_hoeken, sizeof(rode_hoeken), hoektallen, sizeof(hoektallen));

	blauw_driehoek = maakVorm(blauwe_hoeken, sizeof(blauwe_hoeken), hoektallen, sizeof(hoektallen));
}

void tekenLogo() {
	// glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); Standard.
	gebruikVerver(logo_verver);
	zetVerverFloat4v(logo_verver, "voorwerp_kleur", BLAUW);
	tekenVorm(blauw_driehoek);
	zetVerverFloat4v(logo_verver, "voorwerp_kleur", ROOD);
	tekenVorm(rood_driehoek);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(4);
	zetVerverFloat4v(logo_verver, "voorwerp_kleur", WIT);
	tekenVorm(rood_driehoek);
	tekenVorm(blauw_driehoek);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	// Reset.
}
