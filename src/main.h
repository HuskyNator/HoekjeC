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
#define LOOPSNELHEID 1.5 // m/s
#define RENSNELHEID 2.9

Mat4f zichtMatrix;
booleaan zichtMatrixBijgewerkt;

extern int schermbreedte;
extern int schermhoogte;
extern float voorvlak;
extern float achtervlak;
extern float zichthoek;
extern float loopsnelheid;

extern double plekx;
extern double pleky;
extern double plekz;
extern int loopx;
extern int loopz;
extern booleaan rent;

void loop();
void werkPlekMatrixBij();
void werkDraaiMatrixBij();
void werkProjectieMatrixBij();
void werkZichtMatrixBij();

#endif