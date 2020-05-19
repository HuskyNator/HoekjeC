#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

#include "verver.h"

#include "main.h"

#define SCHERM_BREEDTE (1920 / 2)
#define SCHERM_HOOGTE (1080 / 2)

int main()
{
    puts("Hellow");

    // Maak GLFW
    if (!glfwInit())
    {
        puts(":C geen glfw");
        exit(-1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
    printf("Je gebruikt OpenGL versie: %s\n", glGetString(GL_VERSION));
    glViewport(0, 0, SCHERM_BREEDTE, SCHERM_HOOGTE);
    glfwSetWindowSizeCallback(scherm, schermgrootte_terugroep);

    // Zet Toets Terugroep Functie.
    glfwSetKeyCallback(scherm, toets_terugroep);

    Verver *verver = maakVerver("./shaders/kleur.vert", "./shaders/kleur.frag");

    // Maak Driehoek
    float hoeken[] = {
        1, 1, 0,   // RB
        1, 0, 0,   // Rood
        1, -1, 0,  // RO
        0, 0, 1,   // Blauw
        -1, -1, 0, // LO
        1, 0, 1,   // Rood + Blauw
        -1, 1, 0,  // LB
        0, 1, 0    // Groen
    };
    unsigned int driehoek[] = {
        1, 2, 0,
        1, 2, 3};

    // Maak Driehoek VAO
    unsigned int driehoekVAO;
    glGenVertexArrays(1, &driehoekVAO);
    glBindVertexArray(driehoekVAO);

    // Maak Driehoek VBO
    unsigned int driehoekVBO;
    glGenBuffers(1, &driehoekVBO);
    glBindBuffer(GL_ARRAY_BUFFER, driehoekVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(hoeken), hoeken, GL_STATIC_DRAW);

    // Maak Driehoek EBO
    unsigned int driehoekEBO;
    glGenBuffers(1, &driehoekEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, driehoekEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(driehoek), driehoek, GL_STATIC_DRAW);

    // Zet VAO Pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * 3 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glClearColor(0, 0.5, 0, 1);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while (!glfwWindowShouldClose(scherm))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        gebruikVerver(verver);
        float t = glfwGetTime();
        zetVerverInt(verver, "tijd", t);

        glBindVertexArray(driehoekVAO);
        glDrawElements(GL_TRIANGLES, sizeof(driehoek), GL_UNSIGNED_INT, 0);

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
    else *(bestand_string + gelezen) = '\0';

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
