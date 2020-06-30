
#include "verver.h"

#include "bestandslezer.h"
#include "lineair.h"

#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>

#define FOUTMELDING_GROOTTE 512

Verver* maakVerver(const char* hoekVerfLocatie, const char* fragmentVerfLocatie) {
	Verver* verver = malloc(sizeof(Verver));
	char foutmelding[FOUTMELDING_GROOTTE];
	int makenGelukt;

	unsigned int hoekVerf = glCreateShader(GL_VERTEX_SHADER);

	char* hoekVerfString = leesBestand(hoekVerfLocatie);
	glShaderSource(hoekVerf, 1, (const char**)&hoekVerfString, NULL);
	glCompileShader(hoekVerf);
	free(hoekVerfString);

	glGetShaderiv(hoekVerf, GL_COMPILE_STATUS, &makenGelukt);
	if (!makenGelukt) {
		glGetShaderInfoLog(hoekVerf, FOUTMELDING_GROOTTE, NULL, foutmelding);
		fputs(foutmelding, stderr);
		puts("Hoekverf kon niet gemaakt worden.");
		exit(-1);
	}

	unsigned int fragmentVerf = glCreateShader(GL_FRAGMENT_SHADER);
	char* fragmentVerfString = leesBestand(fragmentVerfLocatie);
	glShaderSource(fragmentVerf, 1, (const char**)&fragmentVerfString, NULL);
	glCompileShader(fragmentVerf);
	free(fragmentVerfString);

	glGetShaderiv(fragmentVerf, GL_COMPILE_STATUS, &makenGelukt);
	if (!makenGelukt) {
		glGetShaderInfoLog(hoekVerf, FOUTMELDING_GROOTTE, NULL, foutmelding);
		fputs(foutmelding, stderr);
		puts("Fragmentverf kon niet gemaakt worden.");
		exit(-1);
	}

	unsigned int verfProgramma = glCreateProgram();
	glAttachShader(verfProgramma, hoekVerf);
	glAttachShader(verfProgramma, fragmentVerf);
	glLinkProgram(verfProgramma);

	glGetProgramiv(verfProgramma, GL_LINK_STATUS, &makenGelukt);
	if (!makenGelukt) {
		glGetProgramInfoLog(verfProgramma, FOUTMELDING_GROOTTE, NULL, foutmelding);
		fputs(foutmelding, stderr);
		puts("Verver kon niet gemaakt worden.");
		exit(-1);
	}

	glDeleteShader(hoekVerf);
	glDeleteShader(fragmentVerf);
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

void zetVerverFloat4v(const Verver* verver, const char* naam, float waarden[]) {
	int verwijzing = glGetUniformLocation(verver->verfProgramma, naam);
	glUniform4fv(verwijzing, 1, waarden);
}

void zetVerverMat4f(const Verver* verver, const char* naam, const Mat4f* mat) {
	int verwijzing = glGetUniformLocation(verver->verfProgramma, naam);
	glUniformMatrix4fv(verwijzing, 1, GL_FALSE, (float*)mat);
}