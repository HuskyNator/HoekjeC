#include "bestandslezer.h"
#include "lineair.h"
#include "koppeling.h"
#include "verver.h"
#include "voorwerp.h"
#include "vorm.h"

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int main() {
	puts("Hellow");

	HWND achtergrondScherm = GetConsoleWindow();
	ShowWindow(achtergrondScherm, SW_HIDE);

	opzetten();
	logo();

	Verver* verver = maakVerver("shaders/kleur_voorwerp.vert", "shaders/kleur_voorwerp.frag");
	gebruikVerver(verver);

	Vec3f hoeken[] = {{-1, -1.6, 2}, {1, -1.6, 2}, {0, 0, 2}};
	Vec3ui hoektallen[] = {{0, 1, 2}};
	Vorm* vorm = maakVorm(hoeken, sizeof(hoeken), hoektallen, sizeof(hoektallen));
	Vec3f plaats = {0, 0, 0.1};
	Vec3f grootte = {1, 1, 1};
	Vec3f draai = {0, 0, 0};
	Voorwerp* voorwerpA = maakVoorwerp(vorm, plaats, grootte, draai);
	Vec4f voorwerpKleur = {0.1, 0.8, 0, 1};

	Vec3f vloerHoeken[] = {{-5, -1.6, -5}, {0, -1.6, 5}, {5, -1.6, -5}};
	Vec3ui vloerHoektallen[] = {{0, 1, 2}};
	Vorm* vloerVorm = maakVorm(vloerHoeken, sizeof(vloerHoeken), vloerHoektallen, sizeof(vloerHoektallen));
	Vec3f vloerPlaats = {0, 0, 0};
	Vec3f vloerGrootte = {1, 1, 1};
	Vec3f vloerDraai = {0, 0, 0};
	Voorwerp* vloerVoorwerp = maakVoorwerp(vloerVorm, vloerPlaats, vloerGrootte, vloerDraai);
	Vec4f vloerKleur = {0, 0, 1, 1};

	Vorm* blok = leesObj("vormen/wagen.obj");

	Vec4f blokKleuren[blok->hoek_aantal];
	for (int h = 0; h < blok->hoek_aantal; h++) {
		float r = (float)rand() / RAND_MAX;
		float g = (float)rand() / RAND_MAX;
		float b = (float)rand() / RAND_MAX;
		blokKleuren[h] = (Vec4f){r, g, b, 1};
	}
	vormVoegInhoudToe(blok, (float*)blokKleuren, sizeof(blokKleuren), 4, 1);

	Vec3f blokPlaats = {0, 0, 1};
	Vec3f blokGrootte = {1, 1, 1};
	Vec3f blokDraai = {0, 0, 0};
	Voorwerp* blokVoorwerp = maakVoorwerp(blok, blokPlaats, blokGrootte, blokDraai);
}