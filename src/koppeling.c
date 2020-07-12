#include "koppeling.h"

#include "bestandslezer.h"
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

// Scherm & zicht eigenschappen.

static GLFWwindow* scherm;
static int schermbreedte, schermhoogte;
static float voorvlak = VOORVLAK;
static float achtervlak = ACHTERVLAK;
static float zichthoek = ZICHTHOEK;

static Mat4f projectieMatrix;
static Mat4f draaiMatrix;
static Mat4f plekMatrix;

float loopsnelheid = LOOPSNELHEID;
float rensnelheid = RENSNELHEID;

static double plekx = 0;
static double pleky = 0;
static double plekz = 0;
static int loopx = 0;
static int loopz = 0;
booleaan rent = onwaar;

static double muisplekx;
static double muispleky;
static float draaix = 0;
static float draaiy = 0;
static Vec3f xRichting;
static Vec3f zRichting;

int krijg_schermbreedte() { return schermbreedte; }
int krijg_schermhoogte() { return schermhoogte; }
float krijg_voorvlak() { return voorvlak; }
float krijg_achtervlak() { return achtervlak; }
float krijg_zichthoek() { return zichthoek; }
float zet_voorvlak(float nieuw_voorvlak) {
	voorvlak = nieuw_voorvlak;
	werkProjectieMatrixBij();
}
float zet_achtervlak(float nieuw_achtervlak) {
	achtervlak = nieuw_achtervlak;
	werkProjectieMatrixBij();
}
float zet_zichthoek(float nieuwe_zichthoek) {
	zichthoek = nieuwe_zichthoek;
	werkProjectieMatrixBij();
}

// Tijd van vorige teken oproep.
static double vorigeTijd;
// Tijd tussen laatste en voorlopende teken oproepen.
static double tijdsverschil;

double krijg_TPS() { return 1 / tijdsverschil; }

// Gebruikte opdrachten.

static teken_opdracht tekenaar;
static toets_terugroep_opdracht toets_terugroeper;
static muisplek_terugroep_opdracht muisplek_terugroeper;

// Matrixen

static void werkZichtMatrixBij() {
	zichtMatrix = Mat4fMat4f(projectieMatrix, Mat4fMat4f(draaiMatrix, plekMatrix));
	zichtMatrixBijgewerkt = waar;
}

void werkPlekMatrixBij() {
	plekx += (loopx * xRichting.x + loopz * zRichting.x) * (rent ? rensnelheid : loopsnelheid) * tijdsverschil;
	plekz += (loopx * xRichting.z + loopz * zRichting.z) * (rent ? rensnelheid : loopsnelheid) * tijdsverschil;
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

void werkProjectieMatrixBij() {
	glfwGetWindowSize(scherm, &schermbreedte, &schermhoogte);
	projectieMatrix = perspectiefMatrix(voorvlak, achtervlak, zichthoek, (double)schermbreedte / (double)schermhoogte);
	werkZichtMatrixBij();
}

// // Werk plek bij als er gelopen wordt.
// static void loop() {
// 	if (loopx != 0 || loopz != 0) {
// 		werkPlekMatrixBij();
// 	}
// }

// Terugroepen

static void muisplek_terugroep(GLFWwindow* scherm, double x, double y) {
	gebruikte_muis_terugroeper(x, y);
	// draaiy += x - muisplekx;
	// draaix += y - muispleky;
	// muisplekx = x;
	// muispleky = y;
	// werkDraaiMatrixBij();
}

static void schermgrootte_terugroep(GLFWwindow* scherm, int breedte, int hoogte) {
	glViewport(0, 0, breedte, hoogte);
	schermbreedte = breedte;
	schermhoogte = hoogte;
	werkProjectieMatrixBij();
}

static void toets_terugroep(GLFWwindow* scherm, int toets, int toets2, int handeling, int toevoeging) {
	gebruikte_toets_terugroeper(toets, toets2, handeling, toevoeging);
	// 	if (handeling == GLFW_PRESS) {
	// 		switch (toets) {
	// 			case GLFW_KEY_W:
	// 				loopz += 1;
	// 				break;
	// 			case GLFW_KEY_S:
	// 				loopz -= 1;
	// 				break;
	// 			case GLFW_KEY_A:
	// 				loopx -= 1;
	// 				break;
	// 			case GLFW_KEY_D:
	// 				loopx += 1;
	// 				break;
	// 			case GLFW_KEY_LEFT_SHIFT:
	// 				rent = waar;
	// 				break;
	// 			case GLFW_KEY_ESCAPE:
	// 				glfwSetWindowShouldClose(scherm, GLFW_TRUE);
	// 				break;
	// 			case GLFW_KEY_GRAVE_ACCENT:
	// 				ShowWindow(GetConsoleWindow(), SW_SHOW);
	// 				break;
	// 		}
	// 	} else if (handeling == GLFW_RELEASE) {
	// 		switch (toets) {
	// 			case GLFW_KEY_W:
	// 				loopz -= 1;
	// 				break;
	// 			case GLFW_KEY_S:
	// 				loopz += 1;
	// 				break;
	// 			case GLFW_KEY_A:
	// 				loopx += 1;
	// 				break;
	// 			case GLFW_KEY_D:
	// 				loopx -= 1;
	// 				break;
	// 			case GLFW_KEY_LEFT_SHIFT:
	// 				rent = onwaar;
	// 				break;
	// 		}
	// 	}
}

static void APIENTRY foutmelding_terugroep(GLenum bron, GLenum soort, unsigned int id, GLenum ernstigheid,
										   GLsizei grootte, const char* bericht, const void* gebruikersParameter) {
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;
	fputs("----\n", stderr);
	fprintf(stderr, "Foutmelding #%u: %s\n", id, bericht);
}

// Krijg & zet opdrachten.

// Gebruikte opdrachten.

void zet_teken_opdracht(teken_opdracht opdracht) { tekenaar = opdracht; }
void zet_muisplek_terugroepper(muisplek_terugroep_opdracht opdracht) { muisplek_terugroeper = opdracht; }
void zet_toets_terugroepper(toets_terugroep_opdracht opdracht) { toets_terugroeper = opdracht; }

teken_opdracht krijg_teken_opdracht() { return tekenaar; }
muisplek_terugroep_opdracht krijg_muisplek_terugroepper() { return muisplek_terugroeper; }
toets_terugroep_opdracht krijg_toets_terugroepper() { return toets_terugroeper; }

// Teken opdrachten.

void opzetten() {
	// GLFW
	if (!glfwInit()) {
		puts(":C geen glfw");
		exit(-1);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);

	// Maak Scherm
	scherm = glfwCreateWindow(SCHERM_BREEDTE, SCHERM_HOOGTE, VENSTER_NAAM, NULL, NULL);
	if (scherm == NULL) {
		puts(":C geen scherm");
		exit(-2);
	}
	glfwMakeContextCurrent(scherm);

	// Maak Functies
	int fout = glewInit();
	if (fout != GLEW_OK) {
		fprintf(stderr, "Fout: %s\n", glewGetErrorString(fout));
		exit(-1);
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

	werkProjectieMatrixBij();
	werkDraaiMatrixBij();
	werkPlekMatrixBij();
	glfwGetCursorPos(scherm, &muisplekx, &muispleky);
	vorigeTijd = glfwGetTime();
}

void lus() {
	while (!glfwWindowShouldClose(scherm)) {
		teken();
		glfwPollEvents();
	}

	glfwDestroyWindow(scherm);
	glfwTerminate();
}

void teken() {
	glClearColor(0.15, 0.15, 0.15, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	loop();	 // Werk plaats bij.

	tekenaar();

	glfwSwapBuffers(scherm);

	double nieuweTijd = glfwGetTime();
	tijdsverschil = nieuweTijd - vorigeTijd;
	vorigeTijd = nieuweTijd;

	// Herstel vlag.
	zichtMatrixBijgewerkt = onwaar;
}

void logo() {
	glClearColor(0.15, 0.15, 0.15, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	maakLogo();
	tekenLogo();
	glfwSwapBuffers(scherm);
	verwijderLogo();
	Sleep(2000);
}
