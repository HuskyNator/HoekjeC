#include "HC/booleaan.h"
#include "HC/koppeling.h"
#include "HC/lezers/bestandslezer.h"
#include "HC/verf/verver.h"
#include "HC/voorwerpen/voorwerp.h"
#include "HC/voorwerpen/vorm.h"
#include "HC/wiskunde/lineair.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <GLFW/glfw3.h>

Voorwerp* blokVoorwerp;
Voorwerp* vloerVoorwerp;
Verver* verver;

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
		Mat4f richtingsMatrix = draaiMatrixy(krijg_muisx() * 0.01);
		Vec3f xRichting = Vec4n3f(Mat4fVec4f(richtingsMatrix, (Vec4f){1, 0, 0, 1}), onwaar);
		Vec3f zRichting = Vec4n3f(Mat4fVec4f(richtingsMatrix, (Vec4f){0, 0, 1, 1}), onwaar);
		double snelheid = tijdsverschil * (rent ? RENSNELHEID : LOOPSNELHEID);
		wijzig_plekx(snelheid * (looptx * xRichting.x + looptz * zRichting.x));
		wijzig_plekz(snelheid * (looptx * xRichting.z + looptz * zRichting.z));
		wijzig_pleky(snelheid * loopty);
	}
}

static void tekenaar() { tekenVoorwerp(blokVoorwerp, verver); }

int main() {
	// HWND achtergrondScherm = GetConsoleWindow();
	// ShowWindow(achtergrondScherm, SW_HIDE);

	const char* HC = "HoekjeC ";
	const char* kern_versie = krijg_kern_versie();
	char* window_name = malloc(strlen(HC) + strlen(kern_versie) + 1);
	strcpy(window_name, HC);
	strcat(window_name, kern_versie);
	puts(window_name);

	opzetten();

	zet_schermnaam(window_name);
	free(window_name);

	verver = maakVerver("files/shaders/kleur_voorwerp.vert", "files/shaders/kleur_voorwerp.frag");
	gebruikVerver(verver);

	Vorm* blok = leesObj("files/objects/bugatti.obj");

	Lijst* blokKleuren = maakLijst(blok->hoek_aantal, sizeof(Vec4f));
	for (int h = 0; h < blok->hoek_aantal; h++) {
		float r = (float)rand() / RAND_MAX;
		float g = (float)rand() / RAND_MAX;
		float b = (float)rand() / RAND_MAX;
		Vec4f h = {r, g, b, 1};
		lijstVoeg(blokKleuren, &h);
	}
	vormVoegInhoudToe(blok, blokKleuren, 3);

	Vec3f blokPlaats = {0, 0, 0};
	Vec3f blokGrootte = {1, 1, 1};
	Vec3f blokDraai = {0, 0, 0};
	blokVoorwerp = maakVoorwerp(blok, blokPlaats, blokGrootte, blokDraai);

	zet_toets_terugroeper(toets_terugroeper);
	zet_denker(denker);
	zet_tekenaar(tekenaar);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	lus();
}
