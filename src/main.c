#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "verver.h"
#include "voorwerp.h"
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

    //      Algemeen

    Verver *verver = maakVerver("./shaders/normaal.vert", "./shaders/normaal.frag");

    // Maak Driehoek
    float hoeken[] = {
        -0.9 / SCHERM_BREEDTE * SCHERM_HOOGTE*0.15, 1*0.015, 0,
        -0.7 / SCHERM_BREEDTE * SCHERM_HOOGTE*0.15, -2.5*0.015, 0,
        1.0 / SCHERM_BREEDTE * SCHERM_HOOGTE*0.15, 1*0.015, 0};
    unsigned int hoektallen[] = {
        0, 1, 2};
    float hoeken2[] = {
        (-1.0 / SCHERM_BREEDTE * SCHERM_HOOGTE + 0.1)*0.15, (1+0.05)*0.15, 0,
        (-0.5 / SCHERM_BREEDTE * SCHERM_HOOGTE + 0.1)*0.15, (-1+0.05)*0.15, 0,
        (1.0 / SCHERM_BREEDTE * SCHERM_HOOGTE + 0.1)*0.15, (0.5+0.05)*0.15, 0};

    Voorwerp *driehoek = maakVoorwerp(hoeken, sizeof(hoeken), hoektallen, sizeof(hoektallen));
    Voorwerp *driehoek2 = maakVoorwerp(hoeken2, sizeof(hoeken2), hoektallen, sizeof(hoektallen));

    glEnable(GL_DEPTH_TEST);

    gebruikVerver(verver);
    float voorwerp_kleur[] = {1, 0, 0, 1};
    float voorwerp_kleur2[] = {0, 0, 1, 1};
    while (!glfwWindowShouldClose(scherm))
    {
        glClearColor(0, 0.5, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        gebruikVerver(verver);
        zetVerverFloat4v(verver, "voorwerp_kleur", voorwerp_kleur);
        tekenVoorwerp(driehoek);
        zetVerverFloat4v(verver, "voorwerp_kleur", voorwerp_kleur2);
        tekenVoorwerp(driehoek2);
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
