#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "verver.h"
#include "voorwerp.h"
#include "logo.h"
#include "main.h"

#define SCHERM_BREEDTE (1920 / 2)
#define SCHERM_HOOGTE (1080 / 2)

int main()
{

    puts("Hellow");

    //      GLFW

    if (!glfwInit())
    {
        puts(":C geen glfw");
        exit(-1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);

    // Maak Scherm
    GLFWwindow *scherm = glfwCreateWindow(SCHERM_BREEDTE, SCHERM_HOOGTE, ":3", NULL, NULL);
    if (scherm == NULL)
    {
        puts(":C geen scherm");
        exit(-2);
    }
    glfwMakeContextCurrent(scherm);

    // Maak Functies
    int blij = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    if (!blij)
    {
        puts(":C geen functies");
        exit(-3);
    }

    // Toon Versie & Geef Schermgrootte Aan OpenGL
    glfwSetWindowSizeCallback(scherm, schermgrootte_terugroep);
    // Zet Toets Terugroep Functie.
    glfwSetKeyCallback(scherm, toets_terugroep);

    //      OpenGL

    printf("Je gebruikt OpenGL versie: %s\n", glGetString(GL_VERSION));
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(foutmelding_terugroep, NULL);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
    glViewport(0, 0, SCHERM_BREEDTE, SCHERM_HOOGTE);

    // Maak Logo
    maakLogo();

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(scherm))
    {
        glClearColor(0.15, 0.15, 0.15, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        tekenLogo();

        glfwSwapBuffers(scherm);
        glfwPollEvents();
    }

    //glDeleteProgram(shader_programma);
    //glfwDestroyWindow(scherm);
    glfwTerminate();
}

void lees_bestand(const char *bestand_naam, char *bestand_string, size_t bestand_grootte)
{
    FILE *bestand = fopen(bestand_naam, "r");
    if (bestand == NULL)
    {
        fprintf(stderr, "Bestand '%s' bestaat niet.", bestand_naam);
        return;
    }

    long gelezen = fread(bestand_string, sizeof(char), bestand_grootte, bestand);
    if (gelezen == bestand_grootte)
        *(bestand_string + bestand_grootte - 1) = '\0';
    else
        *(bestand_string + gelezen) = '\0';

    fclose(bestand);
}

void schermgrootte_terugroep(GLFWwindow *scherm, int breedte, int hoogte)
{
    glViewport(0, 0, breedte, hoogte);
}

void toets_terugroep(GLFWwindow *scherm, int toets, int scancode, int handeling, int toevoeging)
{
    if (handeling == GLFW_PRESS)
    {
        switch (toets)
        {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(scherm, GLFW_TRUE);
        }
    }
}

void APIENTRY foutmelding_terugroep(GLenum bron, GLenum soort, unsigned int id, GLenum ernstigheid, GLsizei grootte, const char *bericht, const void *gebruikersParameter)
{
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
        return;
    fputs("----\n", stderr);
    fprintf(stderr, "Foutmelding #%u: %s\n", id, bericht);
}
