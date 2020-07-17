#ifndef LOGO_H
#define LOGO_H

#define SCHERM_BREEDTE (1920.0 / 2.0)
#define SCHERM_HOOGTE (1080.0 / 2.0)
#include "verver.h"
#include "vorm.h"

#include <GL/glew.h>
#include <stdlib.h>

static float ROOD[] = {1, 0, 0, 1};
static float BLAUW[] = {0, 0, 1, 1};
static float WIT[] = {1, 1, 1, 1};

static Verver* logo_verver;
static Vorm* rood_driehoek;
static Vorm* blauw_driehoek;

void maakLogo(/*unsigned int grootte*/) {
	logo_verver = maakVerver("./shaders/normaal.vert", "./shaders/normaal.frag");
	Vec3f rode_hoeken[] = {{-0.9 / SCHERM_BREEDTE * SCHERM_HOOGTE * 0.15 * 2, 1 * 0.015 * 2, -0.1},
								 {-0.7 / SCHERM_BREEDTE * SCHERM_HOOGTE * 0.15 * 2, -2.5 * 0.015 * 2, -0.1},
								 {1.0 / SCHERM_BREEDTE * SCHERM_HOOGTE * 0.15 * 2, 1 * 0.015 * 2, -0.1}};
	Lijst rl = {sizeof(rode_hoeken), 3, sizeof(Vec3f), rode_hoeken};

	Vec3f blauwe_hoeken[] = {
		{(-1.0 / SCHERM_BREEDTE * SCHERM_HOOGTE + 0.1) * 0.15 * 2, (1 + 0.05) * 0.15 * 2, 0},
		{(-0.5 / SCHERM_BREEDTE * SCHERM_HOOGTE + 0.1) * 0.15 * 2, (-1 + 0.05) * 0.15 * 2, 0},
		{(1.0 / SCHERM_BREEDTE * SCHERM_HOOGTE + 0.1) * 0.15 * 2, (0.5 + 0.05) * 0.15 * 2, 0}};
	Lijst bl = {sizeof(blauwe_hoeken), 3, sizeof(Vec3f), blauwe_hoeken};

	Vlak vlakken[] = {{0, 1, 2}};
	Lijst vl = {sizeof(vlakken), 1, sizeof(Vlak), vlakken};

	rood_driehoek = maakVorm(&rl, &vl);

	blauw_driehoek = maakVorm(&bl, &vl);
}

void tekenLogo() {
	glClearColor(0.15, 0.15, 0.15, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

void verwijderLogo() {
	verwijderVerver(logo_verver);
	verwijderVorm(rood_driehoek);
	verwijderVorm(blauw_driehoek);
}

#endif