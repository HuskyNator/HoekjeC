#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

#include "main.h"

#define SCHERM_BREEDTE (1920 / 2)
#define SCHERM_HOOGTE (1080 / 2)
#define SHADER_GROOTTE 512

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

    // Maak Vertex Shader
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    char vertex_shader_string[SHADER_GROOTTE];
    lees_bestand("./shaders/normaal.vert", vertex_shader_string);
    const GLchar *vertex_shader_string_ptr = vertex_shader_string;
    glShaderSource(vertex_shader, 1, &vertex_shader_string_ptr, NULL);
    glCompileShader(vertex_shader);

    // Toets Vertex Shader
    int gelukt;
    char info[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &gelukt);
    if (!gelukt)
    {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info);
        fputs(info, stderr);
        puts(":C geen shader");
        exit(-4);
    }

    // Maak Fragment Shader
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    char fragment_shader_string[SHADER_GROOTTE];
    lees_bestand("./shaders/normaal.frag", fragment_shader_string);
    const GLchar *fragment_shader_string_ptr = fragment_shader_string;
    glShaderSource(fragment_shader, 1, &fragment_shader_string_ptr, NULL);
    glCompileShader(fragment_shader);

    // Toets Fragment Shader
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &gelukt);
    if (!gelukt)
    {
        glGetShaderInfoLog(fragment_shader, 512, NULL, info);
        fputs(info, stderr);
        puts(":C geen shader");
        exit(-4);
    }

    // Maak Shader Programma
    unsigned int shader_programma = glCreateProgram();
    glAttachShader(shader_programma, vertex_shader);
    glAttachShader(shader_programma, fragment_shader);
    glLinkProgram(shader_programma);

    // Toets Shader Programma
    glGetProgramiv(shader_programma, GL_LINK_STATUS, &gelukt);
    if (!gelukt)
    {
        glGetProgramInfoLog(shader_programma, 512, NULL, info);
        fputs(info, stderr);
        puts(":C geen programma");
        exit(-5);
    }

    // Gebruik Shader Programma
    glUseProgram(shader_programma);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // Maak Driehoek
    float hoeken[] = {
        0.5, 0.5, 0, // RB
        0.5, -0.5, 0, // RO
        -0.5, -0.5, 0, // LO
        0.5, 0.5, 0}; // LB
    int driehoek[] = {
        1, 2, 0,
        1, 2, 3
    };

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

    // Maak Driehoek VAO
    unsigned int driehoekVAO;
    glGenVertexArrays(1, &driehoekVAO);
    glBindVertexArray(driehoekVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)0);
    glEnableVertexAttribArray(0);

    glClearColor(0, 0.5, 0, 1);
    while (!glfwWindowShouldClose(scherm))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_programma);
        glBindVertexArray(driehoekVAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(scherm);
        glfwPollEvents();
    }

    //glDeleteProgram(shader_programma);
    //glfwDestroyWindow(scherm);
    glfwTerminate();
}

void lees_bestand(char *bestandsnaam, char *bestand_string)
{
    FILE *bestand = fopen(bestandsnaam, "r");
    if (bestand == NULL)
    {
        fprintf(stderr, "Bestand '%s' bestaat niet.", bestandsnaam);
        return;
    }

    char *schrijfpunt = bestand_string;
    char karakter;
    while ((karakter = fgetc(bestand)) != EOF)
    {
        *schrijfpunt = karakter;
        schrijfpunt += 1;
    }
    *schrijfpunt = '\0';
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
