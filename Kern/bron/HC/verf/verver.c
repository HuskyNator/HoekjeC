
#include "verver.h"

#include "koppeling.h"
#include "lezers/bestandslezer.h"
#include "materiaal.h"
#include "wiskunde/lineair.h"

#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>

#define FOUTMELDING_GROOTTE 512

Verver maakVerver(const char* hoekVerfLocatie, const char* fragmentVerfLocatie) {
	char foutmelding[FOUTMELDING_GROOTTE];
	int makenGelukt;

	unsigned int hoekVerver = glCreateShader(GL_VERTEX_SHADER);
	char* hoekVerverString = leesBestand(hoekVerfLocatie);
	glShaderSource(hoekVerver, 1, (const char**)&hoekVerverString, NULL);
	glCompileShader(hoekVerver);
	free(hoekVerverString);

	glGetShaderiv(hoekVerver, GL_COMPILE_STATUS, &makenGelukt);
	if (!makenGelukt) {
		glGetShaderInfoLog(hoekVerver, FOUTMELDING_GROOTTE, NULL, foutmelding);
		fputs(foutmelding, stderr);
		puts("Hoekverf kon niet gemaakt worden.");
		exit(-1);
	}

	unsigned int fragmentVerver = glCreateShader(GL_FRAGMENT_SHADER);
	char* fragmentVerverString = leesBestand(fragmentVerfLocatie);
	glShaderSource(fragmentVerver, 1, (const char**)&fragmentVerverString, NULL);
	glCompileShader(fragmentVerver);
	free(fragmentVerverString);

	glGetShaderiv(fragmentVerver, GL_COMPILE_STATUS, &makenGelukt);
	if (!makenGelukt) {
		glGetShaderInfoLog(fragmentVerver, FOUTMELDING_GROOTTE, NULL, foutmelding);
		fputs(foutmelding, stderr);
		puts("Fragmentverf kon niet gemaakt worden.");
		exit(-1);
	}

	Verver verver = glCreateProgram();
	glAttachShader(verver, hoekVerver);
	glAttachShader(verver, fragmentVerver);
	glLinkProgram(verver);

	glGetProgramiv(verver, GL_LINK_STATUS, &makenGelukt);
	if (!makenGelukt) {
		glGetProgramInfoLog(verver, FOUTMELDING_GROOTTE, NULL, foutmelding);
		fputs(foutmelding, stderr);
		puts("Verver kon niet gemaakt worden.");
		exit(-1);
	}

	glDeleteShader(hoekVerver);
	glDeleteShader(fragmentVerver);
	return verver;
}

void verwijderVerver(Verver verver) { glDeleteProgram(verver); }

void gebruikVerver(Verver verver) {
	if (krijg_huidige_verver() != verver) glUseProgram(verver);

	zetVerverDouble3v(verver, "zicht_plek", (double*)&plek);
	zetVerverMat4f(verver, "zicht_matrix", &zichtM);
}

void zetVerverUint(Verver verver, const char* naam, unsigned int waarde) {
	int verwijzing = glGetUniformLocation(verver, naam);
	glUniform1ui(verwijzing, waarde);
}

void zetVerverInt(Verver verver, const char* naam, int waarde) {
	int verwijzing = glGetUniformLocation(verver, naam);
	glUniform1i(verwijzing, waarde);
}

void zetVerverFloat(Verver verver, const char* naam, float waarde) {
	int verwijzing = glGetUniformLocation(verver, naam);
	glUniform1f(verwijzing, waarde);
}
void zetVerverFloat3v(Verver verver, const char* naam, float waarden[]) {
	int verwijzing = glGetUniformLocation(verver, naam);
	glUniform3fv(verwijzing, 1, waarden);
}
void zetVerverFloat4v(Verver verver, const char* naam, float waarden[]) {
	int verwijzing = glGetUniformLocation(verver, naam);
	glUniform4fv(verwijzing, 1, waarden);
}

void zetVerverDouble(Verver verver, const char* naam, double waarde) {
	int verwijzing = glGetUniformLocation(verver, naam);
	glUniform1d(verwijzing, waarde);
}
void zetVerverDouble3v(Verver verver, const char* naam, double waarden[]) {
	int verwijzing = glGetUniformLocation(verver, naam);
	glUniform3dv(verwijzing, 1, waarden);
}
void zetVerverDouble4v(Verver verver, const char* naam, double waarden[]) {
	int verwijzing = glGetUniformLocation(verver, naam);
	glUniform4dv(verwijzing, 1, waarden);
}

void zetVerverMat4f(Verver verver, const char* naam, const Mat4f* mat) {
	int verwijzing = glGetUniformLocation(verver, naam);
	glUniformMatrix4fv(verwijzing, 1, GL_FALSE, (float*)mat);
}

void zetVerverMateriaal(Verver verver, Materiaal* materiaal) {
	zetVerverFloat3v(verver, "materiaal.vaste_kleur", &materiaal->vaste_kleur.x);
	zetVerverFloat3v(verver, "materiaal.afweer_kleur", &materiaal->afweer_kleur.x);
	zetVerverFloat3v(verver, "materiaal.weerkaats_kleur", &materiaal->weerkaats_kleur.x);
	zetVerverFloat(verver, "materiaal.weerkaatsing", materiaal->weerkaatsing);
	zetVerverFloat(verver, "materiaal.doorzichtigheid", materiaal->doorzichtigheid);
	zetVerverFloat(verver, "materiaal.brekingsgetal", materiaal->brekingsgetal);
	// TODO verlichtingswijze & naam ?
}