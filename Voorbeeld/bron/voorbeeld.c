#include "HC/booleaan.h"
#include "HC/kleuren.h"
#include "HC/koppeling.h"
#include "HC/lezers/bestandslezer.h"
#include "HC/lijsten/lijst.h"
#include "HC/logo.h"
#include "HC/verf/verver.h"
#include "HC/voorwerpen/vormen.h"
#include "HC/wiskunde/lineair.h"

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define _USE_MATH_DEFINES
#include <math.h>

#ifndef VOORBEELD_VERSIE
#define VOORBEELD_VERSIE "onbekend"
#endif

#define LOOPSNELHEID 1.4
#define RENSNELHEID 3.7

char looptx = 0;
char loopty = 0;
char looptz = 0;
booleaan rent = onwaar;

static void toets_terugroeper(int toets, int toets2, int handeling, int toevoeging) {
	if (handeling == GLFW_PRESS) {
		switch (toets) {
			case GLFW_KEY_A:
				looptx--;
				break;
			case GLFW_KEY_D:
				looptx++;
				break;
			case GLFW_KEY_W:
				looptz++;
				break;
			case GLFW_KEY_S:
				looptz--;
				break;
			case GLFW_KEY_SPACE:
				loopty++;
				break;
			case GLFW_KEY_LEFT_CONTROL:
				loopty--;
				break;
			case GLFW_KEY_LEFT_SHIFT:
				rent = waar;
				break;
			case GLFW_KEY_ESCAPE:
				sluit_scherm();
				break;
		}
	} else if (handeling == GLFW_RELEASE) {
		switch (toets) {
			case GLFW_KEY_A:
				looptx++;
				break;
			case GLFW_KEY_D:
				looptx--;
				break;
			case GLFW_KEY_W:
				looptz--;
				break;
			case GLFW_KEY_S:
				looptz++;
				break;
			case GLFW_KEY_SPACE:
				loopty--;
				break;
			case GLFW_KEY_LEFT_CONTROL:
				loopty++;
				break;
			case GLFW_KEY_LEFT_SHIFT:
				rent = onwaar;
				break;
		}
	}
}

static void denker(double tijdsverschil) {
	if (looptx != 0 || looptz != 0 || loopty != 0) {
		Mat4f richtingsMatrix = draaiMatrixy(krijg_muisx() * krijg_muisgevoeligheid());
		Vec3f xRichting = Vec4n3f(Mat4fVec4f(richtingsMatrix, (Vec4f){1, 0, 0, 1}), onwaar);
		Vec3f zRichting = Vec4n3f(Mat4fVec4f(richtingsMatrix, (Vec4f){0, 0, 1, 1}), onwaar);
		double snelheid = tijdsverschil * (rent ? RENSNELHEID : LOOPSNELHEID);
		wijzig_plekx(snelheid * (looptx * xRichting.x + looptz * zRichting.x));
		wijzig_plekz(snelheid * (looptx * xRichting.z + looptz * zRichting.z));
		wijzig_pleky(snelheid * loopty);
	}
}

Verver verver;
Lijst* tekenlijst;

static void tekenaar() {
	gebruikVerver(verver);
	const unsigned int tel = tekenlijst->tel;
	for (unsigned int i = 0; i < tel; i++) {
		voorwerpTeken(lijstKrijg(tekenlijst, i, Voorwerp*), verver);
	}
}

int main() {
	// HWND achtergrondScherm = GetConsoleWindow();
	// ShowWindow(achtergrondScherm, SW_HIDE);

	// Versie:
	const char* versie_vorm = "HoekjeC %s - Voorbeeld %s :3";
	const char* kern_versie = krijg_kern_versie();
	const char* voorbeeld_versie = VOORBEELD_VERSIE;
	char* versie = malloc(strlen(versie_vorm) - 4 + strlen(kern_versie) + strlen(voorbeeld_versie) + 1);
	sprintf(versie, versie_vorm, kern_versie, voorbeeld_versie);
	fputs("\n> ", stdout);
	fputs(versie, stdout);
	fputs("\n\n", stdout);

	// Kern Opzetten:
	opzetten();

	zet_schermnaam(versie);
	free(versie);

	zet_toets_terugroeper(toets_terugroeper);
	zet_denker(denker);
	zet_tekenaar(tekenaar);

	// Wereld:
	verver = krijgKleurVerver();
	tekenlijst = maakLijst(3, sizeof(Voorwerp*));

	Driehoek* driehoek = maakKant();
	lijstVoeg(tekenlijst, &driehoek);
	voorwerpZetPlek(driehoek, (Vec3f){-0.5, -0.5, -1.01});

	Vierkant* vierkant = maakVierkant();
	lijstVoeg(tekenlijst, &vierkant);
	voorwerpZetDraai(vierkant, (Vec3f){M_PI_2, 0, 0});
	voorwerpZetGrootte(vierkant, (Vec3f){1000, 1000, 1000});
	voorwerpZetPlek(vierkant, (Vec3f){0, -0.51, 0});
	vormZetKleur(vierkant, &DonkerGrijs);

	Blok* blok = maakBlok();
	lijstVoeg(tekenlijst, &blok);
	voorwerpZetGrootte(blok, (Vec3f){1, 1, 2});
	vormZetKleur(blok, &Blauw);
	blok->vormgegevens->rand=waar;

	//TODO: Ontbreekt?
	Groep* logo = maakLogo((Vec3f){0, 0, 0}, (Vec3f){1, 1, 1}, (Vec3f){0, 0, 0});
	lijstVoeg(tekenlijst, &logo);

	// Beginnen:
	lus();
}
