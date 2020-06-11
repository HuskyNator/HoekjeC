
#include <glad/glad.h>
#include <stdlib.h>
#include <stdio.h>

#include "main.h"
#include "verver.h"

Verver *maakVerver(const char *hoekVerfLocatie, const char *fragmentVerfLocatie)
{
    Verver *verver = malloc(sizeof(Verver));
    char verfString[VERVER_GROOTTE];
    const GLchar *verfStringVerwijzing = verfString;
    int makenGelukt;

    unsigned int hoekVerf = glCreateShader(GL_VERTEX_SHADER);

    lees_bestand(hoekVerfLocatie, verfString, VERVER_GROOTTE);
    glShaderSource(hoekVerf, 1, &verfStringVerwijzing, NULL);
    glCompileShader(hoekVerf);

    glGetShaderiv(hoekVerf, GL_COMPILE_STATUS, &makenGelukt);
    if (!makenGelukt)
    {
        glGetShaderInfoLog(hoekVerf, VERVER_GROOTTE, NULL, verfString);
        fputs(verfString, stderr);
        puts("Hoekverf kon niet gemaakt worden.");
        exit(-4); // TODO schoonmaken?
    }

    unsigned int fragmentVerf = glCreateShader(GL_FRAGMENT_SHADER);
    lees_bestand(fragmentVerfLocatie, verfString, VERVER_GROOTTE);
    glShaderSource(fragmentVerf, 1, &verfStringVerwijzing, NULL);
    glCompileShader(fragmentVerf);

    glGetShaderiv(fragmentVerf, GL_COMPILE_STATUS, &makenGelukt);
    if (!makenGelukt)
    {
        glGetShaderInfoLog(fragmentVerf, VERVER_GROOTTE, NULL, verfString);
        fputs(verfString, stderr);
        puts("Fragmentverf kon niet gemaakt worden.");
        exit(-4); // TODO schoonmaken?
    }

    unsigned int verfProgramma = glCreateProgram();
    glAttachShader(verfProgramma, hoekVerf);
    glAttachShader(verfProgramma, fragmentVerf);
    glLinkProgram(verfProgramma);

    glGetProgramiv(verfProgramma, GL_LINK_STATUS, &makenGelukt);
    if (!makenGelukt)
    {
        glGetProgramInfoLog(verfProgramma, VERVER_GROOTTE, NULL, verfString);
        fputs(verfString, stderr);
        puts("Verver kon niet gemaakt worden.");
        exit(-5); // TODO schoonmaken?
    }

    glDeleteShader(hoekVerf);
    glDeleteShader(fragmentVerf);
    verver->verfProgramma = verfProgramma;
    return verver;
}

void verwijderVerver(Verver *verver)
{
    glDeleteProgram(verver->verfProgramma);
    free(verver);
}

void gebruikVerver(Verver *verver)
{
    glUseProgram(verver->verfProgramma);
}

void zetVerverInt(Verver *verver, const char *naam, int waarde)
{
    int verwijzing = glGetUniformLocation(verver->verfProgramma, naam);
    glUniform1i(verwijzing, waarde);
}

void zetVerverFloat(Verver *verver, const char *naam, float waarde)
{
    int verwijzing = glGetUniformLocation(verver->verfProgramma, naam);
    glUniform1f(verwijzing, waarde);
}

void zetVerverFloat4v(Verver *verver, const char *naam, float waarden[])
{
    int verwijzing = glGetUniformLocation(verver->verfProgramma, naam);
    glUniform4fv(verwijzing, 1, waarden);
}
