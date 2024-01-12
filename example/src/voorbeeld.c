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

static void muisknop_terugroeper(int knop, int handeling, int toevoeging) {
	if (handeling == GLFW_PRESS) {
		switch(knop){
			case GLFW_MOUSE_BUTTON_LEFT:
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				break;
			case GLFW_MOUSE_BUTTON_RIGHT:
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				break;
		}
	}
}

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

int main(int argc, char* argv[]) {
	if(argc == 0){
		puts("argv[0] is missing");
		return -2;
	}

	char* dir_path = findDir(argv[0]);

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

	char* vert_path = strConcat(dir_path, "files/shaders/kleur_voorwerp.vert");
	char* frag_path = strConcat(dir_path, "files/shaders/kleur_voorwerp.frag");

	verver = maakVerver(vert_path, frag_path);
	gebruikVerver(verver);
	free(vert_path);
	free(frag_path);

	char* default_obj_path = strConcat(dir_path, "files/objects/bugatti.obj");
	const char* obj_path = (argc>1)?argv[1]:default_obj_path;
	Vorm* vorm = leesObj(obj_path);
	free(default_obj_path);

	Vec3f vormPlaats = {0, 0, 0};
	Vec3f vormGrootte = {1, 1, 1};
	Vec3f vormDraai = {0, 0, 0};
	blokVoorwerp = maakVoorwerp(vorm, vormPlaats, vormGrootte, vormDraai);

	zet_toets_terugroeper(toets_terugroeper);
	zet_muisknop_terugroeper(muisknop_terugroeper);
	zet_denker(denker);
	zet_tekenaar(tekenaar);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	lus();
}
