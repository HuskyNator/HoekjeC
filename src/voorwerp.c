#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>
#include "voorwerp.h"

Voorwerp *maakVoorwerp(float hoeken[], size_t hoekengrootte, unsigned int hoektallen[], size_t hoektallengrootte)
{
    Voorwerp *voorwerp = malloc(sizeof(voorwerp));
    if (hoektallengrootte % 3 != 0)
    {
        fputs("Hoektalaantal niet een veelvoud van 3.", stderr);
    }
    voorwerp->grootte = hoektallengrootte / sizeof(unsigned int);

    glCreateVertexArrays(1, &voorwerp->VAO);
    glBindVertexArray(voorwerp->VAO);

    glCreateBuffers(1, &voorwerp->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, voorwerp->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, hoektallengrootte, hoektallen, GL_STATIC_DRAW);

    unsigned int VBO;

    glCreateBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, hoekengrootte, hoeken, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glDeleteBuffers(1, &VBO);
    return voorwerp;
}

void voorwerpVoegToe(Voorwerp *voorwerp, float inhoud[], size_t inhoudsgrootte, unsigned int onderdeelgrootte, unsigned int standplaats)
{
    unsigned int VBO;
    glCreateBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, inhoudsgrootte, inhoud, GL_STATIC_DRAW);

    glBindVertexArray(voorwerp->VAO);
    glVertexAttribPointer(standplaats, onderdeelgrootte, GL_FLOAT, GL_FALSE, onderdeelgrootte * sizeof(float), (void *)0);
    glEnableVertexAttribArray(standplaats);

    glBindVertexArray(0);
    glDeleteBuffers(1, &VBO);
}

void tekenVoorwerp(Voorwerp *voorwerp)
{
    glBindVertexArray(voorwerp->VAO);
    glDrawElements(GL_TRIANGLES, voorwerp->grootte, GL_UNSIGNED_INT, 0);
}

void verwijderVoorwerp(Voorwerp *voorwerp)
{
    glDeleteVertexArrays(1, &voorwerp->VAO);
    free(voorwerp);
}
