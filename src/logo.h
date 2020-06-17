
#include "verver.h"
#include "voorwerp.h"

#include <GL/glew.h>
#include <stdlib.h>

#define SCHERM_BREEDTE (1920 / 2)
#define SCHERM_HOOGTE (1080 / 2)

static float ROOD[] = {1, 0, 0, 1};
static float BLAUW[] = {0, 0, 1, 1};
static float WIT[] = {1, 1, 1, 1};

static Voorwerp* rood_driehoek;
static Voorwerp* blauw_driehoek;
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

	rood_driehoek = maakVoorwerp(rode_hoeken, sizeof(rode_hoeken), hoektallen, sizeof(hoektallen));

	blauw_driehoek = maakVoorwerp(blauwe_hoeken, sizeof(blauwe_hoeken), hoektallen, sizeof(hoektallen));
}

void tekenLogo() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gebruikVerver(logo_verver);
	zetVerverFloat4v(logo_verver, "voorwerp_kleur", BLAUW);
	tekenVoorwerp(blauw_driehoek);
	zetVerverFloat4v(logo_verver, "voorwerp_kleur", ROOD);
	tekenVoorwerp(rood_driehoek);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(4);
	zetVerverFloat4v(logo_verver, "voorwerp_kleur", WIT);
	tekenVoorwerp(rood_driehoek);
	tekenVoorwerp(blauw_driehoek);
}
