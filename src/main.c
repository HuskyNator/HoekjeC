#include "main.h"

#include "lineair.h"
#include "logo.h"
#include "verver.h"
#include "voorwerp.h"

#define _USE_MATH_DEFINES
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int schermbreedte = SCHERM_BREEDTE;
int schermhoogte = SCHERM_HOOGTE;
float voorvlak = VOORVLAK;
float achtervlak = ACHTERVLAK;
float zichthoek = ZICHTHOEK;
float loopsnelheid = LOOPSNELHEID;

double plekx = 0;
double pleky = 0;
double plekz = 0;
int loopx = 0;
int loopz = 0;

static double muisplekx;
static double muispleky;
static float draaix = 0;
static float draaiy = 0;
static Vec3f xRichting;
static Vec3f zRichting;

static Mat4f projectieMatrix;
static Mat4f draaiMatrix;
static Mat4f plekMatrix;

void muisplek_terugroep(GLFWwindow* scherm, double x, double y) {
	draaiy += x - muisplekx;
	draaix += y - muispleky;
	muisplekx = x;
	muispleky = y;
	werkDraaiMatrixBij();
}

static void schermgrootte_terugroep(GLFWwindow* scherm, int breedte, int hoogte) {
	glViewport(0, 0, breedte, hoogte);
	schermbreedte = breedte;
	schermhoogte = hoogte;
	werkProjectieMatrixBij();
}

static void toets_terugroep(GLFWwindow* scherm, int toets, int scancode, int handeling, int toevoeging) {
	if (handeling == GLFW_PRESS) {
		switch (toets) {
			case GLFW_KEY_W:
				loopz += 1;
				break;
			case GLFW_KEY_S:
				loopz -= 1;
				break;
			case GLFW_KEY_A:
				loopx -= 1;
				break;
			case GLFW_KEY_D:
				loopx += 1;
				break;
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(scherm, GLFW_TRUE);
				break;
			case GLFW_KEY_GRAVE_ACCENT:
				ShowWindow(GetConsoleWindow(), SW_SHOW);
				break;
		}
	} else if (handeling == GLFW_RELEASE) {
		switch (toets) {
			case GLFW_KEY_W:
				loopz -= 1;
				break;
			case GLFW_KEY_S:
				loopz += 1;
				break;
			case GLFW_KEY_A:
				loopx += 1;
				break;
			case GLFW_KEY_D:
				loopx -= 1;
				break;
		}
	}
}

static void APIENTRY foutmelding_terugroep(GLenum bron, GLenum soort, unsigned int id, GLenum ernstigheid,
										   GLsizei grootte, const char* bericht, const void* gebruikersParameter) {
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;
	fputs("----\n", stderr);
	fprintf(stderr, "Foutmelding #%u: %s\n", id, bericht);
}

int main() {
	puts("Hellow");

	HWND achtergrondScherm = GetConsoleWindow();
	ShowWindow(achtergrondScherm, SW_HIDE);

	//      GLFW

	if (!glfwInit()) {
		puts(":C geen glfw");
		exit(-1);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);

	// Maak Scherm
	GLFWwindow* scherm = glfwCreateWindow(SCHERM_BREEDTE, SCHERM_HOOGTE, ":3", NULL, NULL);
	if (scherm == NULL) {
		puts(":C geen scherm");
		exit(-2);
	}
	glfwMakeContextCurrent(scherm);

	// Maak Functies
	int fout = glewInit();
	if (fout != GLEW_OK) {
		fprintf(stderr, "Fout: %s\n", glewGetErrorString(fout));
		exit(-3);
	}

	// Toon Versie & Geef Schermgrootte Aan OpenGL
	glfwSetWindowSizeCallback(scherm, schermgrootte_terugroep);
	// Zet Toets Terugroep Functie.
	glfwSetKeyCallback(scherm, toets_terugroep);
	// Zet Muis Terugroep Functie.
	glfwSetInputMode(scherm, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(scherm, muisplek_terugroep);

	//      OpenGL

	printf("Je gebruikt OpenGL versie: %s\n", glGetString(GL_VERSION));
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(foutmelding_terugroep, NULL);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
	glViewport(0, 0, SCHERM_BREEDTE, SCHERM_HOOGTE);
	glEnable(GL_DEPTH_TEST);

	// Daadwerkelijk.

	werkProjectieMatrixBij();
	werkDraaiMatrixBij();
	werkPlekMatrixBij();
	glfwGetCursorPos(scherm, &muisplekx, &muispleky);

	Verver* verver = maakVerver("shaders/voorwerp.vert", "shaders/normaal.frag");
	gebruikVerver(verver);

	Vec3f hoeken[] = {{-1, -1, 2}, {1, -1, 2}, {0, 0, 2}};
	Vec3ui hoektallen[] = {{0, 1, 2}};
	Vorm* vorm = maakVorm((float*)&hoeken, sizeof(hoeken), (unsigned int*)&hoektallen, sizeof(hoektallen));
	Vec3f plaats = {0, 0, 0.1};
	Vec3f grootte = {1, 1, 1};
	Vec3f draai = {0, 0, 0};
	Voorwerp* voorwerpA = maakVoorwerp(vorm, plaats, grootte, draai);
	Vec4f voorwerpKleur = {0.1, 0.8, 0, 1};

	Vec3f vloerHoeken[] = {{-5, -1, -5}, {0, -1, 5}, {5, -1, -5}};
	Vec3ui vloerHoektallen[] = {{0, 1, 2}};
	Vorm* vloerVorm =
		maakVorm((float*)&vloerHoeken, sizeof(vloerHoeken), (unsigned int*)&vloerHoektallen, sizeof(vloerHoektallen));
	Vec3f vloerPlaats = {0, 0, 0};
	Vec3f vloerGrootte = {1, 1, 1};
	Vec3f vloerDraai = {0, 0, 0};
	Voorwerp* vloerVoorwerp = maakVoorwerp(vloerVorm, vloerPlaats, vloerGrootte, vloerDraai);
	Vec4f vloerKleur = {0, 0, 1, 1};

	glClearColor(0.15, 0.15, 0.15, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	maakLogo();
	tekenLogo();
	glfwSwapBuffers(scherm);
	verwijderLogo();
	Sleep(2000);

	gebruikVerver(verver);
	while (!glfwWindowShouldClose(scherm)) {
		glClearColor(0.15, 0.15, 0.15, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		loop();

		zetVerverFloat4v(verver, "voorwerp_kleur", (float*)&voorwerpKleur);
		tekenVoorwerp(voorwerpA, verver);
		zetVerverFloat4v(verver, "voorwerp_kleur", (float*)&vloerKleur);
		tekenVoorwerp(vloerVoorwerp, verver);

		zichtMatrixBijgewerkt = onwaar;
		glfwSwapBuffers(scherm);
		glfwPollEvents();
	}

	verwijderVorm(vorm);
	verwijderVorm(vloerVorm);
	verwijderVerver(verver);

	glfwDestroyWindow(scherm);
	glfwTerminate();
}

void loop() {
	if (loopx != 0 || loopz != 0) {
		werkPlekMatrixBij();
	}
}

void werkPlekMatrixBij() {
	plekx += loopx * xRichting.x * loopsnelheid + loopz * zRichting.x * loopsnelheid;
	plekz += loopx * xRichting.z * loopsnelheid + loopz * zRichting.z * loopsnelheid;
	plekMatrix = verplaatsMatrix(-plekx, -pleky, -plekz);
	werkZichtMatrixBij();
}

void werkDraaiMatrixBij() {
	Mat4f draaixMatrix = draaiMatrixx(-draaix * 0.01);
	Mat4f draaiyMatrix = draaiMatrixy(-draaiy * 0.01);
	draaiMatrix = Mat4fMat4f(draaixMatrix, draaiyMatrix);
	werkZichtMatrixBij();
	Mat4f richtingMatrix = draaiMatrixy(draaiy * 0.01);
	xRichting = Vec4n3f(Mat4fVec4f(richtingMatrix, (Vec4f){1, 0, 0, 1}), waar);
	zRichting = Vec4n3f(Mat4fVec4f(richtingMatrix, (Vec4f){0, 0, 1, 1}), waar);
}

void werkProjectieMatrixBij() {	 // Gebeurt niet vaak.
	projectieMatrix = perspectiefMatrix(voorvlak, achtervlak, zichthoek, (double)schermhoogte / (double)schermbreedte);
	werkZichtMatrixBij();
}

void werkZichtMatrixBij() {
	zichtMatrix = Mat4fMat4f(projectieMatrix, Mat4fMat4f(draaiMatrix, plekMatrix));
	zichtMatrixBijgewerkt = waar;
}

void lees_bestand(const char* bestand_naam, char* bestand_string, size_t bestand_grootte) {
	FILE* bestand = fopen(bestand_naam, "r");
	if (bestand == NULL) {
		fprintf(stderr, "Bestand '%s' bestaat niet.", bestand_naam);
		return;
	}

	long gelezen = fread(bestand_string, sizeof(char), bestand_grootte, bestand);
	if (gelezen == bestand_grootte) *(bestand_string + bestand_grootte - 1) = '\0';
	else
		*(bestand_string + gelezen) = '\0';

	fclose(bestand);
}
