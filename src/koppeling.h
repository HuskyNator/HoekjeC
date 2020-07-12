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
#define LOOPSNELHEID 1.5  // m/s
#define RENSNELHEID 2.9

#define VENSTER_NAAM ":3"

// Zicht.

/**
 * Matrix bestaande uit de projectie, draai en plek matrixen.
 * Vertaalt de wereld, zo dat het zicht gedraaid en verplaatst kan zijn, naar het eenheidsblok.
 * (Het wordt geprojecteerd en behoud een relatieve z-diepte)
 * De projectie matrix maakt gebruik van de volgende getallen, waarvoor krijg & zet opdrachten aanwezig zijn:
 * - voorvlak, vanaf hoe ver vanaf de zichtplek gezien kan worden.
 * - achtervlak, tot hoe ver vanaf de zichtplek gezien kan worden.
 * - zichthoek, de horizontale hoek van het zicht.
 */
Mat4f zichtMatrix;

/**
 * Vlag die aangeeft dat de zichtmatrix bijgewerkt is.
 * Wordt gebruikt door oa. voorwerpen om hun opgeslagen tekenmatrix enkel bij te werken indien nodig.
 */
booleaan zichtMatrixBijgewerkt;

// Krijg & zet opdrachten.

int krijg_schermbreedte();
int krijg_schermhoogte();
float krijg_voorvlak();
float krijg_achtervlak();
float krijg_zichthoek();
float zet_voorvlak();
float zet_achtervlak();
float zet_zichthoek();

// Krijg aantal teken oproepen per seconde.
double krijg_TPS();

// Gebruikte opdrachten.

// Een opdracht die voorwerpen tekent.
typedef void (*teken_opdracht)();
// Een opdracht die muisverplaatsing verwerkt.
typedef void (*muisplek_terugroep_opdracht)(double x, double y);
// Een opdracht die toetsinvoer verwerkt.
typedef void (*toets_terugroep_opdracht)(int toets, int toets2, int handeling, int toevoeging);

// Een opdracht die de projectie matrix levert.
typedef Mat4f (*projectie_opdracht)(float voorvlak, float achtervlak, float zichthoek, double schermverhouding);
// Een opdracht die de draai matrix levert.
typedef Mat4f (*draai_opdracht)(double x, double y);

// Zet welke teken_opdracht gebruikt wordt
void zet_teken_opdracht(teken_opdracht opdracht);
// Zet welke muisplek_terugroep_opdracht gebruikt moet worden.
void zet_muisplek_terugroepper(muisplek_terugroep_opdracht muisplekterugroepper);
// Zet welke toets_terugroep_opdracht gebruikt moet worden.
void zet_toets_terugroepper(toets_terugroep_opdracht toetsterugroepper);

// Krijg de gebruikte teken_opdracht.
teken_opdracht krijg_teken_opdracht();
// Krijgt de gebruikte muisplek_terugroep_opdracht.
muisplek_terugroep_opdracht krijg_muisplek_terugroepper();
// Krijgt de gebruikte toets_terugroep_opdracht.
toets_terugroep_opdracht krijg_toets_terugroepper();

// Teken opdrachten.

/**
 * Zet het scherm & glfw en opengl op.
 * Moet gebruikt worden voor andere dingen gedaan worden.
 */
void opzetten();

/**
 * Lust over de tekenopdracht & verwerkt invoer.
 */
void lus();

/**
 * Tekent één maal het scherm, dmv. de tekenopdracht.
 */
void teken();

/**
 * Tekent het HoekjeC logo.
 */
void logo();

#endif