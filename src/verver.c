
#include "verver.h"

#include "bestandslezer.h"
#include "lineair.h"
#include "materiaal.h"

#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>

#define FOUTMELDING_GROOTTE 512

Verver* maakVerver(const char* hoekVerfLocatie, const char* fragmentVerfLocatie) {
	Verver* verver = malloc(sizeof(Verver));
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

	unsigned int verfProgramma = glCreateProgram();
	glAttachShader(verfProgramma, hoekVerver);
	glAttachShader(verfProgramma, fragmentVerver);
	glLinkProgram(verfProgramma);

	glGetProgramiv(verfProgramma, GL_LINK_STATUS, &makenGelukt);
	if (!makenGelukt) {
		glGetProgramInfoLog(verfProgramma, FOUTMELDING_GROOTTE, NULL, foutmelding);
		fputs(foutmelding, stderr);
		puts("Verver kon niet gemaakt worden.");
		exit(-1);
	}

	glDeleteShader(hoekVerver);
	glDeleteShader(fragmentVerver);
	verver->verfProgramma = verfProgramma;
	return verver;
}

void verwijderVerver(Verver* verver) {
	glDeleteProgram(verver->verfProgramma);
	free(verver);
}

void gebruikVerver(const Verver* verver) { glUseProgram(verver->verfProgramma); }

void zetVerverInt(const Verver* verver, const char* naam, int waarde) {
	int verwijzing = glGetUniformLocation(verver->verfProgramma, naam);
	glUniform1i(verwijzing, waarde);
}

void zetVerverFloat(const Verver* verver, const char* naam, float waarde) {
	int verwijzing = glGetUniformLocation(verver->verfProgramma, naam);
	glUniform1f(verwijzing, waarde);
}
void zetVerverFloat3v(const Verver* verver, const char* naam, float waarden[]) {
	int verwijzing = glGetUniformLocation(verver->verfProgramma, naam);
	glUniform3fv(verwijzing, 1, waarden);
}
void zetVerverFloat4v(const Verver* verver, const char* naam, float waarden[]) {
	int verwijzing = glGetUniformLocation(verver->verfProgramma, naam);
	glUniform4fv(verwijzing, 1, waarden);
}

void zetVerverDouble(const Verver* verver, const char* naam, double waarde) {
	int verwijzing = glGetUniformLocation(verver->verfProgramma, naam);
	glUniform1d(verwijzing, waarde);
}
void zetVerverDouble3v(const Verver* verver, const char* naam, double waarden[]) {
	int verwijzing = glGetUniformLocation(verver->verfProgramma, naam);
	glUniform3dv(verwijzing, 1, waarden);
}
void zetVerverDouble4v(const Verver* verver, const char* naam, double waarden[]) {
	int verwijzing = glGetUniformLocation(verver->verfProgramma, naam);
	glUniform4dv(verwijzing, 1, waarden);
}

void zetVerverMat4f(const Verver* verver, const char* naam, const Mat4f* mat) {
	int verwijzing = glGetUniformLocation(verver->verfProgramma, naam);
	glUniformMatrix4fv(verwijzing, 1, GL_FALSE, (float*)mat);
}

void zetVerverMateriaal(Verver* verver, Materiaal* materiaal) {
	zetVerverFloat3v(verver, "materiaal.vaste_kleur", &materiaal->vaste_kleur.x);
	zetVerverFloat3v(verver, "materiaal.afweer_kleur", &materiaal->afweer_kleur.x);
	zetVerverFloat3v(verver, "materiaal.weerkaats_kleur", &materiaal->weerkaats_kleur.x);
	zetVerverFloat(verver, "materiaal.weerkaatsing", materiaal->weerkaatsing);
	zetVerverFloat(verver, "materiaal.doorzichtigheid", materiaal->doorzichtigheid);
	zetVerverFloat(verver, "materiaal.brekingsgetal", materiaal->brekingsgetal);
	// TODO verlichtingswijze & naam ?
}