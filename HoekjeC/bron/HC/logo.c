#include "logo.h"

static Vierhoek* vierhoek;
static Driehoek* rood_driehoek;
static Driehoek* blauw_driehoek;

static const Vec3f vlak_plekken[] = {{0, 0, 0}, {1.7, 0, 0}, {1.7, 1, 0}, {0, 1, 0}};
static const Vec3f rood_plekken[] = {{0.9 / 5.0, 0.5, 0}, {1.2 / 5.0, 2.0 / 5.0, 0}, {3.9 / 5.0, 0, 0}};
static const Vec3f blauw_plekken[] = {{1.0 / 5.0, 4.0 / 5.0, 0}, {4.2 / 5.0, 0.9 / 5.0, 0}, {1.8 / 5.0, 3.2 / 5.0, 0}};

static const Vec3f normalen[] = {{0, 0, -1}, {0, 0, -1}, {0, 0, -1}, {0, 0, -1}};

// TODO voeg offset toe in maak_voorwerp_bruikbaar naar plekken als verfplekken niet gegeven worden.
static const Vec2f vlak_verfplekken[] = {{0, 0}, {1.7, 0}, {1.7, 1}, {0, 1}};
static const Vec2f rood_verfplekken[] = {{0.9 / 5.0, 0.5}, {1.2 / 5.0, 2.0 / 5.0}, {3.9 / 5.0, 0}};
static const Vec2f blauw_verfplekken[] = {{1.0 / 5.0, 4.0 / 5.0}, {4.2 / 5.0, 0.9 / 5.0}, {1.8 / 5.0, 3.2 / 5.0}};

static const Vec4f vierhoek_kleur = {0.15, 0.15, 0.15, 1};

static unsigned int VAO;
static unsigned int VBOs[3];
static unsigned int EBO;
static booleaan opgezet;

static void opzet() {
	// logo_verver = krijgKleurVerver();

	vierhoek = maakVierhoek(vlak_plekken, normalen, vlak_verfplekken);
	rood_driehoek = maakDriehoek(rood_plekken, normalen, rood_verfplekken);
	blauw_driehoek = maakDriehoek(blauw_plekken, normalen, blauw_verfplekken);

	vormZetKleur(vierhoek, &vierhoek_kleur);
	vormZetKleur(rood_driehoek, &Rood);
	vormZetKleur(blauw_driehoek, &Blauw);
}

// TODO: maak tot 'standaard' voorwerp, zodat je de hoeken kunt hergebruiken.
Groep* maakLogo(Vec3f plek, Vec3f grootte, Vec3f draaiing) {
	if (!opgezet) opzet();

	Groep* groep = maakGroep(waar);

	Vierhoek* vierhoek_kopie = malloc(sizeof(Vierhoek));
	Driehoek* rood_driehoek_kopie = malloc(sizeof(Driehoek));
	Driehoek* blauw_driehoek_kopie = malloc(sizeof(Driehoek));
	memcpy(vierhoek_kopie, vierhoek, sizeof(Vierhoek));
	memcpy(rood_driehoek_kopie, rood_driehoek, sizeof(Driehoek));
	memcpy(blauw_driehoek_kopie, blauw_driehoek, sizeof(Driehoek));

	groepVoeg(groep, vierhoek_kopie);
	groepVoeg(groep, rood_driehoek_kopie);
	groepVoeg(groep, blauw_driehoek_kopie);

	voorwerpZetPlek(groep, plek);
	voorwerpZetGrootte(groep, grootte);
	voorwerpZetDraai(groep, draaiing);

	return groep;
}
