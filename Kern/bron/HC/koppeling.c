#include "koppeling.h"

#include "lezers/bestandslezer.h"
#include "logo.h"
#include "verf/verver.h"
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

#define SCHERM_BREEDTE (1920.0 / 2.0)
#define SCHERM_HOOGTE (1080.0 / 2.0)
#define VOORVLAK 0.1
#define ACHTERVLAK 50.0
#define ZICHTHOEK 100.0

/**		SCHERM		**/

static GLFWwindow* scherm;
int schermbreedte = SCHERM_BREEDTE;
int schermhoogte = SCHERM_HOOGTE;
double schermverhouding = (double)SCHERM_BREEDTE / (double)SCHERM_HOOGTE;

void zet_schermnaam(const char* naam) { glfwSetWindowTitle(scherm, naam); }
int krijg_schermbreedte() { return schermbreedte; }
int krijg_schermhoogte() { return schermhoogte; }
double krijg_schermverhouding() { return schermverhouding; }

static void glfw_fout_terugroep(int fout_soort, const char* melding) {
	fputs(melding, stderr);
	putc('\n', stderr);
}

static void schermgrootte_terugroep(GLFWwindow* scherm, int breedte, int hoogte) {
	glViewport(0, 0, breedte, hoogte);
	schermbreedte = breedte;
	schermhoogte = hoogte;
	bijwerkbaar_projectieM = waar;
}

/**		OPENGL		**/

static void APIENTRY foutmelding_terugroep(GLenum bron, GLenum soort, unsigned int id, GLenum ernstigheid, GLsizei grootte,
										   const char* bericht, const void* gebruikersParameter) {
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;
	fputs("----\n", stderr);
	fprintf(stderr, "Foutmelding #%u: %s\n", id, bericht);
}

/**		ZICHT MATRIXEN		**/

Mat4f plekM;
Mat4f draai1M;
Mat4f draai2M;
Mat4f draai3M;
Mat4f draaiM;
Mat4f projectieM;
Mat4f zichtM;

booleaan bijwerkbaar_plekM = waar;
booleaan bijwerkbaar_draai1M = waar;
booleaan bijwerkbaar_draai2M = waar;
booleaan bijwerkbaar_draai3M = waar;
booleaan bijwerkbaar_draaiM = waar;
booleaan bijwerkbaar_projectieM = waar;
booleaan bijwerkbaark_zichtM = waar;

void werk_M_bij() {
	if (bijwerkbaar_plekM) werk_plekM_bij();
	if (bijwerkbaar_draaiM) werk_draaiM_bij();
	if (bijwerkbaar_projectieM) werk_projectieM_bij();
	if (bijwerkbaark_zichtM) werk_zichtM_bij();
}

/*	PLEK	*/

inline Mat4f standaard_plek_maker(double plekx, double pleky, double plekz) { return verplaatsMatrix(-plekx, -pleky, -plekz); }

static plek_maak_opdracht plek_maker = standaard_plek_maker;
Vec3d plek;

void zet_plek_maker(plek_maak_opdracht opdracht) {
	plek_maker = (opdracht == NULL) ? standaard_plek_maker : opdracht;
	bijwerkbaar_plekM = waar;
}
void zet_plekx(double nieuw) {
	plek.x = nieuw;
	bijwerkbaar_plekM = waar;
}
void zet_pleky(double nieuw) {
	plek.y = nieuw;
	bijwerkbaar_plekM = waar;
}
void zet_plekz(double nieuw) {
	plek.z = nieuw;
	bijwerkbaar_plekM = waar;
}
void zet_plek(Vec3d nieuw) {
	plek = nieuw;
	bijwerkbaar_plekM = waar;
}
void wijzig_plekx(double verandering) {
	plek.x += verandering;
	bijwerkbaar_plekM = waar;
}
void wijzig_pleky(double verandering) {
	plek.y += verandering;
	bijwerkbaar_plekM = waar;
}
void wijzig_plekz(double verandering) {
	plek.z += verandering;
	bijwerkbaar_plekM = waar;
}
void wijzig_plek(Vec3d verandering) {
	Vec3dP(plek, verandering);
	bijwerkbaar_plekM = waar;
}

void werk_plekM_bij() {
	plekM = plek_maker(plek.x, plek.y, plek.z);
	bijwerkbaar_plekM = onwaar;
	bijwerkbaark_zichtM = waar;
}

/*	DRAAI	*/

static draai_maak_opdracht draai1_maker = standaard_draaiz_maker;
static draai_maak_opdracht draai2_maker = standaard_draaiy_maker;
static draai_maak_opdracht draai3_maker = standaard_draaix_maker;

inline Mat4f standaard_draaix_maker(double muisx, double muisy) { return draaiMatrixx(-muisy * muisgevoeligheid); }
inline Mat4f standaard_draaiy_maker(double muisx, double muisy) { return draaiMatrixy(-muisx * muisgevoeligheid); }
inline Mat4f standaard_draaiz_maker(double muisx, double muisy) { return identiteitsMatrix; }

void zet_draai1_maker(draai_maak_opdracht opdracht) {
	draai1_maker = (opdracht == NULL) ? standaard_draaiy_maker : opdracht;
	bijwerkbaar_draai1M = waar;
}
void zet_draai2_maker(draai_maak_opdracht opdracht) {
	draai2_maker = (opdracht == NULL) ? standaard_draaix_maker : opdracht;
	bijwerkbaar_draai2M = waar;
}
void zet_draai3_maker(draai_maak_opdracht opdracht) {
	draai3_maker = (opdracht == NULL) ? standaard_draaiz_maker : opdracht;
	bijwerkbaar_draai3M = waar;
}

void werk_draaiM_bij() {
	draai1M = draai1_maker(muisx, muisy);
	draai2M = draai2_maker(muisx, muisy);
	draai3M = draai3_maker(muisx, muisy);
	draaiM = Mat4fMat4f(draai3M, Mat4fMat4f(draai2M, draai1M));
	bijwerkbaar_draaiM = onwaar;
	bijwerkbaark_zichtM = waar;
}

/*	PROJECTIE	*/

inline Mat4f standaard_projectie_maker(float voorvlak, float achtervlak, float zichthoek, double schermverhouding) {
	return perspectiefMatrix(voorvlak, achtervlak, zichthoek, schermverhouding);
}

static projectie_maak_opdracht projectie_maker = standaard_projectie_maker;
float voorvlak = VOORVLAK;
float achtervlak = ACHTERVLAK;
float zichthoek = ZICHTHOEK;

void zet_projectie_maker(projectie_maak_opdracht opdracht) {
	projectie_maker = (opdracht == NULL) ? standaard_projectie_maker : opdracht;
	bijwerkbaar_projectieM = waar;
}
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

void werk_projectieM_bij() {
	projectieM = projectie_maker(voorvlak, achtervlak, zichthoek, schermverhouding);
	bijwerkbaar_projectieM = onwaar;
	bijwerkbaark_zichtM = waar;
}

/*	ZICHT	*/

void werk_zichtM_bij() {
	zichtM = Mat4fMat4f(projectieM, Mat4fMat4f(draaiM, plekM));
	bijwerkbaark_zichtM = onwaar;
}

/**		MUIS		**/

/*	MUISPLEK	*/

double muisx;
double muisy;
float muisgevoeligheid = 0.01;
double muisgrens = M_PI_2;

void zet_muis(double x, double y) {
	glfwSetCursorPos(scherm, x, y);
	muisx = x;
	muisy = y;
	bijwerkbaar_draaiM = waar;
}
void zet_muisgevoeligheid(float gevoeligheid) {
	float verhouding = muisgevoeligheid / gevoeligheid;
	muisx *= verhouding;
	muisy *= verhouding;
	muisgevoeligheid = gevoeligheid;
}
void zet_muisgrens(double grens) { muisgrens = grens; }

static void muisplek_terugroep(GLFWwindow* scherm, double x, double y) {
	muisx = x;
	if (fabs(muisy + y) * muisgevoeligheid < muisgrens) muisy = y;
	bijwerkbaar_draaiM = waar;
}

/*	MUISKNOP	*/

static muisknop_terugroep_opdracht muisknop_terugroeper;
void standaard_muisknop_terugroeper(int knop, int handeling, int toevoeging) {}

void zet_muisknop_terugroeper(muisknop_terugroep_opdracht opdracht) {
	muisknop_terugroeper = (opdracht == NULL) ? standaard_muisknop_terugroeper : opdracht;
}

static void muisknop_terugroep(GLFWwindow* scherm, int knop, int handeling, int toevoeging) {
	muisknop_terugroeper(knop, handeling, toevoeging);
}

/**		TOETSEN		**/

static toets_terugroep_opdracht toets_terugroeper;
void standaard_toets_terugroeper(int toets, int toets2, int handeling, int toevoeging) {}

void zet_toets_terugroeper(toets_terugroep_opdracht opdracht) {
	toets_terugroeper = (opdracht == NULL) ? standaard_toets_terugroeper : opdracht;
}

static void toets_terugroep(GLFWwindow* scherm, int toets, int toets2, int handeling, int toevoeging) {
	toets_terugroeper(toets, toets2, handeling, toevoeging);
}

/**		TPS		**/

double vorigeTijd;
double tijdsverschil;
double krijg_TPS() { return 1 / tijdsverschil; }

/**		VERF		**/

static Verver huidige_verver;  // Huidige verver in gebruik.
Verver krijg_huidige_verver() { return huidige_verver; }

/**		ACHTERGROND		**/

Vec4f achtergrond_kleur;

/**		TEKENEN		**/
/*	DENKER	*/

static denk_opdracht denker;
void standaard_denker(double tijdsverschil) {}

void zet_denker(denk_opdracht opdracht) { denker = (opdracht == NULL) ? standaard_denker : opdracht; }

/*	TEKENAAR	*/

static teken_opdracht tekenaar;
void standaard_tekenaar() {}

void zet_tekenaar(teken_opdracht opdracht) { tekenaar = (opdracht == NULL) ? standaard_tekenaar : opdracht; }

/*	BESTURING	*/

void opzetten() {
	// # GLFW
	glfwSetErrorCallback(&glfw_fout_terugroep);
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

	// > Achtergrond
	achtergrond_kleur = DonkerGrijs;

	// > Terugroepen
	zet_toets_terugroeper(NULL);
	glfwSetKeyCallback(scherm, toets_terugroep);
	zet_muisknop_terugroeper(NULL);
	glfwSetMouseButtonCallback(scherm, muisknop_terugroep);

	glfwSetCursorPosCallback(scherm, muisplek_terugroep);
	glfwSetInputMode(scherm, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	zet_muis(0, 0);

	glfwSetWindowSizeCallback(scherm, schermgrootte_terugroep);

	// > Matrixen
	werk_M_bij();

	// > Tekenen
	zet_denker(NULL);
	zet_tekenaar(NULL);
	glLineWidth(4);
}

static void afbreken() {
	glfwDestroyWindow(scherm);
	glfwTerminate();
}

static void stap() {
	glClearColor(achtergrond_kleur.x, achtergrond_kleur.y, achtergrond_kleur.z, achtergrond_kleur.z);
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
		stap();
		glfwPollEvents();
	}
	afbreken();
}

// TODO: Vernieuwen.
// void logo(float tijd) {
// 	maakLogo((Vec3f){0, 0, 0}, (Vec3f){1000, 1000, 1}, (Vec3f){0, 0, 0});
// 	tekenLogo();
// 	glfwSwapBuffers(scherm);
// 	Sleep(tijd);
// 	verwijderLogo();
// }

void sluit_scherm() { glfwSetWindowShouldClose(scherm, waar); }
