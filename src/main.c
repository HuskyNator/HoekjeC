#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "oneigen/stb_image.h"

#include "verver.h"
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

    Verver *verver = maakVerver("./shaders/afbeelding.vert", "./shaders/schilderij.frag");

    // Maak Driehoek
    float hoeken[] = {
        -1, -1, 0,
        1, -1, 0,
        -1, 1, 0,
        0.5, 0.5, 0};
    float afbeeldingplek[] = {
        0, 0,
        1, 0,
        0, 1,
        1, 1};
    unsigned int driehoek[] = {
        0, 1, 2, 1, 3, 2};
    unsigned int driehoekVAO, hoekVBO, afbeeldingVBO, driehoekEBO, afbeelding, muur_afbeelding;

    int breedte, hoogte, kanaalaantal;
    stbi_set_flip_vertically_on_load(1);

    glGenTextures(1, &afbeelding);
    glBindTexture(GL_TEXTURE_2D, afbeelding);
    // float grenskleur[] = {0, 0.5, 0.75, 1.0};
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, grenskleur);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // TODO Antisotropic?

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);

    unsigned char *afbeelding_inhoud = stbi_load("afbeelding.png", &breedte, &hoogte, &kanaalaantal, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, breedte, hoogte, 0, GL_RGBA, GL_UNSIGNED_BYTE, afbeelding_inhoud);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(afbeelding_inhoud);

    glGenTextures(1, &muur_afbeelding);
    glBindTexture(GL_TEXTURE_2D, muur_afbeelding);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, grenskleur);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    afbeelding_inhoud = stbi_load("muur.jpg", &breedte, &hoogte, &kanaalaantal, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, breedte, hoogte, 0, GL_RGB, GL_UNSIGNED_BYTE, afbeelding_inhoud);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(afbeelding_inhoud);

    // Maak Driehoek VAO
    glGenVertexArrays(1, &driehoekVAO);
    glBindVertexArray(driehoekVAO);

    // Maak Driehoek VBO's
    glGenBuffers(1, &hoekVBO);
    glBindBuffer(GL_ARRAY_BUFFER, hoekVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(hoeken), hoeken, GL_STATIC_DRAW);

    glGenBuffers(1, &afbeeldingVBO);
    glBindBuffer(GL_ARRAY_BUFFER, afbeeldingVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(afbeeldingplek), afbeeldingplek, GL_STATIC_DRAW);

    // Maak Driehoek EBO
    glGenBuffers(1, &driehoekEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, driehoekEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(driehoek), driehoek, GL_STATIC_DRAW);

    // Zet VAO Pointers
    glBindBuffer(GL_ARRAY_BUFFER, hoekVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, afbeeldingVBO);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);

    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST);

    gebruikVerver(verver);
    zetVerverInt(verver, "muur_afbeelding", 0);
    zetVerverInt(verver, "afbeelding", 1);
    while (!glfwWindowShouldClose(scherm))
    {
        glClearColor(0, 0.5, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, muur_afbeelding);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, afbeelding);

        gebruikVerver(verver);
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
