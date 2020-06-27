#ifndef MAIN_H
#define MAIN_H
#include "lineair.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define SCHERM_BREEDTE (1920.0 / 2.0)
#define SCHERM_HOOGTE (1080.0 / 2.0)
#define VOORVLAK 0.1
#define ACHTERVLAK 50.0
#define ZICHTHOEK 100.0

Mat4f TIJDELIJK;

Mat4f projectieZichtMatrix;
booleaan projectieZichtMatrixBijgewerkt;
int schermbreedte;
int schermhoogte;
float voorvlak;
float achtervlak;
float zichthoek;

void werkZichtMatrixBij();
void werkProjectieMatrixBij();

/**
 * @brief  Leest bestand naar string.
 * @param  *bestand_naam: de naam van het bestand.
 * @param  *bestand_string: de buffer waar naar geschreven moet wordt.
 * @param  bestand_grootte: grootte van de buffer.
 * @retval None
 */
void lees_bestand(const char* bestand_naam, char* bestand_string, size_t bestand_grootte);

#endif