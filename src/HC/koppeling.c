#include "koppeling.h"

#include "lezers/bestandslezer.h"
#include "logo.h"
#include "verf/verver.h"
#include "voorwerpen/voorwerp.h"
#include "wiskunde/lineair.h"

#define _USE_MATH_DEFINES
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#ifndef KERN_VERSIE
#define KERN_VERSIE "onbekend"
#endif
char* krijg_kern_versie() { return KERN_VERSIE; }

/**		GLFW		**/

static GLFWwindow* scherm;

/**		ZICHT		**/

static booleaan bijwerkbaar_projectieM = waar;
static booleaan bijwerkbaar_draaiM = waar;
static booleaan bijwerkbaar_plekM = waar;
static boolean bijwerkbaark_zichtM = waar;

/**		KRIJG & ZET OPDRACHTEN		**/
/*	SCHERM EIGENSCHAPPEN	*/

static int schermbreedte = SCHERM_BREEDTE;
static int schermhoogte = SCHERM_HOOGTE;
static double schermverhouding = (double)SCHERM_BREEDTE / (double)SCHERM_HOOGTE;

int krijg_schermbreedte() { return schermbreedte; }
int krijg_schermhoogte() { return schermhoogte; }
double krijg_schermverhouding() { return schermverhouding; }
void zet_schermnaam(const char* naam) { glfwSetWindowTitle(scherm, naam); }

/*	PROJECTIE EIGENSCHAPPEN	*/

static float voorvlak = VOORVLAK;
static float achtervlak = ACHTERVLAK;
static float zichthoek = ZICHTHOEK;

float krijg_voorvlak() { return voorvlak; }
float krijg_achtervlak() { return achtervlak; }
float krijg_zichthoek() { return zichthoek; }

void zet_voorvlak(float nieuw) {
	voorvlak = nieuw;
	bijwerkbaar_projectieM = waar;
}
void zet_achtervlak(float nieuw) {
	achtervlak = nieuw;
	bijwerkbaar_projectieM = waar;
}
void zet_zichthoek(float nieuw) {
	zichthoek = nieuw;
	bijwerkbaar_projectieM = waar;
}

/*	PLEK	*/

static double plekx = 0;
static double pleky = 0;
static double plekz = 0;

double krijg_plekx() { return plekx; }
double krijg_pleky() { return pleky; }
double krijg_plekz() { return plekz; }
Vec3d krijg_plek() { return (Vec3d){plekx, pleky, plekz}; }
void zet_plekx(double nieuw) {
	plekx = nieuw;
	bijwerkbaar_plekM = waar;
}
void zet_pleky(double nieuw) {
	pleky = nieuw;
	bijwerkbaar_plekM = waar;
}
void zet_plekz(double nieuw) {
	plekz = nieuw;
	bijwerkbaar_plekM = waar;
}
void zet_plek(double x, double y, double z) {
	plekx = x;
	pleky = y;
	plekz = z;
	bijwerkbaar_plekM = waar;
}

void wijzig_plekx(double verandering) {
	plekx += verandering;
	bijwerkbaar_plekM = waar;
}
void wijzig_pleky(double verandering) {
	pleky += verandering;
	bijwerkbaar_plekM = waar;
}
void wijzig_plekz(double verandering) {
	plekz += verandering;
	bijwerkbaar_plekM = waar;
}
void wijzig_plek(double vx, double vy, double vz) {
	plekx += vx;
	pleky += vy;
	plekz += vz;
	bijwerkbaar_plekM = waar;
}

/*	MUISPLEK	*/

static double muisx;
static double muisy;
double krijg_muisx() { return muisx; }
double krijg_muisy() { return muisy; }
void zet_muis(double x, double y) {
	glfwSetCursorPos(scherm, x, y);
	muisx = x;
	muisy = y;
	bijwerkbaar_draaiM = waar;
}

/*	TPS	*/

// Tijd van vorige teken oproep.
static double vorigeTijd;
// Tijd tussen laatste en voorlopende teken oproepen.
static double tijdsverschil;
double krijg_TPS() { return 1 / tijdsverschil; }

/**		TERUGROEPEN		**/
/*	TOETSEN	*/
static toets_terugroep_opdracht toets_terugroeper;
static void toets_terugroep(GLFWwindow* scherm, int toets, int toets2, int handeling, int toevoeging) {
	toets_terugroeper(toets, toets2, handeling, toevoeging);
}

void zet_toets_terugroeper(toets_terugroep_opdracht opdracht) {
	toets_terugroeper = (opdracht == NULL) ? standaard_toets_terugroeper : opdracht;
}
void standaard_toets_terugroeper(int toets, int toets2, int handeling, int toevoeging) {}

/*	MUIS	*/

static void muisplek_terugroep(GLFWwindow* scherm, double x, double y) {
	muisx = x;
	muisy = y;
	bijwerkbaar_draaiM = waar;
}

/*	SCHERM	*/

static void schermgrootte_terugroep(GLFWwindow* scherm, int breedte, int hoogte) {
	glViewport(0, 0, breedte, hoogte);
	schermbreedte = breedte;
	schermhoogte = hoogte;
	bijwerkbaar_projectieM = waar;
}

/*	OPENGL	*/

static void APIENTRY foutmelding_terugroep(GLenum bron, GLenum soort, unsigned int id, GLenum ernstigheid,
										   GLsizei grootte, const char* bericht, const void* gebruikersParameter) {
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;
	fputs("----\n", stderr);
	fprintf(stderr, "Foutmelding #%u: %s\n", id, bericht);
}

/**		MATRIXEN 		**/
/*	MAKEN	*/

static projectie_maak_opdracht projectie_maker = standaard_projectie_maker;
static draai_maak_opdracht draai_maker = standaard_draai_maker;
static plek_maak_opdracht plek_maker = standaard_plek_maker;

void zet_projectie_maker(projectie_maak_opdracht opdracht) {
	projectie_maker = (opdracht == NULL) ? standaard_projectie_maker : opdracht;
	bijwerkbaar_projectieM = waar;
}
void zet_draai_maker(draai_maak_opdracht opdracht) {
	draai_maker = (opdracht == NULL) ? standaard_draai_maker : opdracht;
	bijwerkbaar_draaiM = waar;
}
void zet_plek_maker(plek_maak_opdracht opdracht) {
	plek_maker = (opdracht == NULL) ? standaard_plek_maker : opdracht;
	bijwerkbaar_plekM = waar;
}

/*	STANDAARD	*/

Mat4f standaard_projectie_maker(float voorvlak, float achtervlak, float zichthoek, double schermverhouding) {
	return perspectiefMatrix(voorvlak, achtervlak, zichthoek, schermverhouding);
}

Mat4f standaard_draai_maker(double muisx, double muisy) {
	Mat4f draaixMatrix = draaiMatrixx(-muisy * 0.01);
	Mat4f draaiyMatrix = draaiMatrixy(-muisx * 0.01);
	return Mat4fMat4f(draaixMatrix, draaiyMatrix);
}

Mat4f standaard_plek_maker(double plekx, double pleky, double plekz) { return verplaatsMatrix(-plekx, -pleky, -plekz); }

/*	BIJWERKEN	*/

static void werk_projectieM_bij() {
	projectieM = projectie_maker(voorvlak, achtervlak, zichthoek, schermverhouding);
	bijwerkbaar_projectieM = onwaar;
	bijwerkbaark_zichtM = waar;
}
static void werk_draaiM_bij() {
	draaiM = draai_maker(muisx, muisy);
	bijwerkbaar_draaiM = onwaar;
	bijwerkbaark_zichtM = waar;
}
static void werk_plekM_bij() {
	plekM = plek_maker(plekx, pleky, plekz);
	bijwerkbaar_plekM = onwaar;
	bijwerkbaark_zichtM = waar;
}

static void werk_zichtM_bij() {
	zichtM = Mat4fMat4f(projectieM, Mat4fMat4f(draaiM, plekM));
	bijwerkbaark_zichtM = onwaar;
}

void werk_M_bij() {
	if (bijwerkbaar_plekM) werk_plekM_bij();
	if (bijwerkbaar_draaiM) werk_draaiM_bij();
	if (bijwerkbaar_projectieM) werk_projectieM_bij();
	if (bijwerkbaark_zichtM) werk_zichtM_bij();
}

/**		TEKENEN		**/
/*	DENKER	*/

static denk_opdracht denker;
void zet_denker(denk_opdracht opdracht) { denker = (opdracht == NULL) ? standaard_denker : opdracht; }
void standaard_denker(double tijdsverschil) {}

/*	TEKENAAR	*/

static teken_opdracht tekenaar;
void zet_tekenaar(teken_opdracht opdracht) { tekenaar = (opdracht == NULL) ? standaard_tekenaar : opdracht; }
void standaard_tekenaar() {}

/*	BESTURING	*/

void opzetten() {
	// # GLFW
	if (!glfwInit()) {
		puts(":C geen glfw");
		exit(-1);
	}

	// > OpenGL
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);

	// > Maak Scherm
	scherm = glfwCreateWindow(SCHERM_BREEDTE, SCHERM_HOOGTE, KERN_VERSIE, NULL, NULL);
	if (scherm == NULL) {
		puts(":C geen scherm");
		exit(-2);
	}
	glfwMakeContextCurrent(scherm);

	// # OpenGL
	// > GLEW
	int fout = glewInit();
	if (fout != GLEW_OK) {
		fprintf(stderr, "Fout: %s\n", glewGetErrorString(fout));
		exit(-1);
	}

	// > OpenGL
	printf("Je gebruikt OpenGL versie: %s\n", glGetString(GL_VERSION));
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(foutmelding_terugroep, NULL);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
	glViewport(0, 0, SCHERM_BREEDTE, SCHERM_HOOGTE);
	glEnable(GL_DEPTH_TEST);

	// # HoekjeC
	// > TPS
	vorigeTijd = glfwGetTime();
	tijdsverschil = 1;

	// > Terugroepen
	zet_toets_terugroeper(NULL);
	glfwSetKeyCallback(scherm, toets_terugroep);

	glfwSetCursorPosCallback(scherm, muisplek_terugroep);
	glfwSetInputMode(scherm, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	zet_muis(0, 0);

	glfwSetWindowSizeCallback(scherm, schermgrootte_terugroep);

	// > Matrixen
	werk_M_bij();

	// > Tekenen
	zet_denker(NULL);
	zet_tekenaar(NULL);
}

static void afbreken() {
	glfwDestroyWindow(scherm);
	glfwTerminate();
}

static void teken() {
	glClearColor(0.15, 0.15, 0.15, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	double nieuweTijd = glfwGetTime();
	tijdsverschil = nieuweTijd - vorigeTijd;
	vorigeTijd = nieuweTijd;

	denker(tijdsverschil);
	werk_M_bij();
	tekenaar();

	glfwSwapBuffers(scherm);
}

void lus() {
	while (!glfwWindowShouldClose(scherm)) {
		teken();
		glfwPollEvents();
	}
	afbreken();
}

void logo(float tijd) {
	maakLogo();
	tekenLogo();
	glfwSwapBuffers(scherm);
	Sleep(tijd);
	verwijderLogo();
}

void sluit_scherm() { glfwSetWindowShouldClose(scherm, waar); }
