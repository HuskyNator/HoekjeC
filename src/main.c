#include "main.h"

#include "lineair.h"
#include "logo.h"
#include "verver.h"
#include "voorwerp.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define SCHERM_BREEDTE (1920 / 2)
#define SCHERM_HOOGTE (1080 / 2)
#define VOORVLAK 0.1
#define ACHTERVLAK 100
#define ZICHTHOEK 100
#define DELTA 0.5

float draaix = 0;
float draaiy = 0;
boolean gedraaid = false;

int main() {
	puts("Hellow");

	// Sleep(1000);
	// ShowWindow(GetConsoleWindow(), SW_HIDE);

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
	maakLogo();

	Mat4f zichtMatrix = identiteitsMatrix();
	Mat4f projectieMatrix = perspectiefMatrix(VOORVLAK, ACHTERVLAK, ZICHTHOEK, SCHERM_BREEDTE / SCHERM_HOOGTE);

	Verver* verver = maakVerver("shaders/voorwerp.vert", "shaders/normaal.frag");
	gebruikVerver(verver);
	zetVerverMat4f(verver, "projectie_matrix", &projectieMatrix);
	zetVerverMat4f(verver, "zichtMatrix", &zichtMatrix);

	Vec3f hoeken[] = {{-1, -1, -1}, {-1, -1, 1}, {-1, 1, -1}, {-1, 1, 1},
					  {1, -1, -1},	{1, -1, 1},	 {1, 1, -1},  {1, 1, 1}};
	Vec4ui hoektallen[] = {{1, 0, 2}, {1, 2, 3}, {2, 0, 4}, {2, 4, 6}, {4, 5, 6}, {5, 7, 6},
						   {5, 1, 7}, {1, 3, 7}, {1, 4, 0}, {1, 5, 4}, {3, 2, 6}, {3, 6, 7}};
	Vorm* vorm = maakVorm((float*)&hoeken, sizeof(hoeken), (unsigned int*)&hoektallen, sizeof(hoektallen));
	Vec3f plaats = {0, 0, -2};
	Vec3f grootte = {1, 1, 1};
	Voorwerp voorwerpA = maakVoorwerp(vorm, plaats, grootte, 0, 0, 0);
	Vec4f voorwerpKleur = {0.1, 0.8, 0, 1};

	zetVerverFloat4v(verver, "voorwerp_kleur", (float*)&voorwerpKleur);

	glClearColor(0.15, 0.15, 0.15, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	tekenLogo();
	glfwSwapBuffers(scherm);
	Sleep(2000);

	while (!glfwWindowShouldClose(scherm)) {
		glClearColor(0.15, 0.15, 0.15, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (gedraaid) {
			gedraaid = false;
			zichtMatrix = vermenigvuldigMatrix(draaiMatrixx(-draaix), draaiMatrixy(-draaiy));
			zetVerverMat4f(verver, "zichtMatrix", &zichtMatrix);
		}

		gebruikVerver(verver);
		tekenVoorwerp(&voorwerpA, verver);

		glfwSwapBuffers(scherm);
		glfwPollEvents();
	}

	// glDeleteProgram(shader_programma);
	// glfwDestroyWindow(scherm);
	glfwTerminate();
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

void schermgrootte_terugroep(GLFWwindow* scherm, int breedte, int hoogte) { glViewport(0, 0, breedte, hoogte); }

void toets_terugroep(GLFWwindow* scherm, int toets, int scancode, int handeling, int toevoeging) {
	if (handeling == GLFW_PRESS) {
		switch (toets) {
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(scherm, GLFW_TRUE);
		}
	}
}

void muisplek_terugroep(GLFWwindow* scherm, double x, double y) {
	static double plekx = SCHERM_BREEDTE / 2;
	static double pleky = SCHERM_HOOGTE / 2;
	draaiy -= plekx - x;
	draaix += pleky - y;
	plekx = x;
	pleky = y;
	gedraaid = true;
}

void APIENTRY foutmelding_terugroep(GLenum bron, GLenum soort, unsigned int id, GLenum ernstigheid, GLsizei grootte,
									const char* bericht, const void* gebruikersParameter) {
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;
	fputs("----\n", stderr);
	fprintf(stderr, "Foutmelding #%u: %s\n", id, bericht);
}
