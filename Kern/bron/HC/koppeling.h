#ifndef KOPPELING_H
#define KOPPELING_H
#include "wiskunde/lineair.h"
#include "verf/verver.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define SCHERM_BREEDTE (1920.0 / 2.0)
#define SCHERM_HOOGTE (1080.0 / 2.0)
#define VOORVLAK 0.1
#define ACHTERVLAK 50.0
#define ZICHTHOEK 100.0

/**		ZICHT		**/

/**
 * De zicht matrix bestaat uit de aaneenschakeling van de projectie, draai en plek matrixen.
 */
Mat4f zichtM, projectieM, draaiM, plekM;

/**		KRIJG & ZET OPDRACHTEN		**/
/*		HOEKJEC		*/

char* krijg_kern_versie();

/*	SCHERM EIGENSCHAPPEN	*/

int krijg_schermbreedte();
int krijg_schermhoogte();
double krijg_schermverhouding();
void zet_schermnaam(const char* naam);

/*	PROJECTIE EIGENSCHAPPEN	*/

float krijg_voorvlak();
float krijg_achtervlak();
float krijg_zichthoek();
void zet_voorvlak(float nieuw);
void zet_achtervlak(float nieuw);
void zet_zichthoek(float nieuw);

/*	PLEK	*/

double krijg_plekx();
double krijg_pleky();
double krijg_plekz();
Vec3d krijg_plek();
void zet_plekx(double nieuw);
void zet_pleky(double nieuw);
void zet_plekz(double nieuw);
void zet_plek(double x, double y, double z);
void wijzig_plekx(double verandering);
void wijzig_pleky(double verandering);
void wijzig_plekz(double verandering);
void wijzig_plek(double vx, double vy, double vz);

/*	MUISPLEK	*/

double krijg_muisx();
double krijg_muisy();
void zet_muis(double x, double y);

/*	TPS	*/

// Krijg aantal teken oproepen per seconde.
double krijg_TPS();

/*	VERF	*/

// Krijg huidig gebruikte verver.
Verver krijg_huidige_verver();

/**		TERUGROEPEN		**/
/*	TOETSEN	*/

typedef void (*toets_terugroep_opdracht)(int toets, int toets2, int handeling, int toevoeging);
void zet_toets_terugroeper(toets_terugroep_opdracht opdracht);
void standaard_toets_terugroeper(int toets, int toets2, int handeling, int toevoeging);

/**		MATRIXEN		**/
/*	MAKEN	*/

// Een opdracht die de projectie matrix levert. De doorgegeven waarden zijn niet bindend.
typedef Mat4f (*projectie_maak_opdracht)(float voorvlak, float achtervlak, float zichthoek, double schermverhouding);
// Een opdracht die de draai matrix levert.
typedef Mat4f (*draai_maak_opdracht)(double muisx, double muisy);
// Eeen opdracht die de plek matrix levert.
typedef Mat4f (*plek_maak_opdracht)(double plekx, double pleky, double plekz);

void zet_projectie_maker(projectie_maak_opdracht opdracht);
void zet_draai_maker(draai_maak_opdracht opdracht);
void zet_plek_maker(plek_maak_opdracht opdracht);

/*	STANDAARD	*/

Mat4f standaard_projectie_maker(float voorvlak, float achtervlak, float zichthoek, double schermverhouding);
Mat4f standaard_draai_maker(double muisx, double muisy);
Mat4f standaard_plek_maker(double plekx, double pleky, double plekz);

/*	BIJWERKEN	*/

// Werk alle matrixen bij, indien nodig.
void werk_M_bij();

/**		TEKENEN		**/
/*	DENKER	*/

typedef void (*denk_opdracht)(double tijdsverschil);
/**
 * Is verantwoordelijk voor het doen van denkstappen voor het programma.
 * Hier onder vallen verplaatsingen en voorwerp schepping/verwijdering.
 * Wordt opgeroepen voor (bijgewerkte) voorwerpen worden getekend.
 */
void zet_denker(denk_opdracht opdracht);
void standaard_denker(double tijdsverschil);

/*	TEKENAAR	*/

typedef void (*teken_opdracht)();
/**
 * Is verantwoordelijk voor het tekenen van voorwerpen.
 * Het aanpassen van plekken / draaiïngen van dezen heeft mogelijk geen gevolg.
 * Wordt opgeroepen na oproeping van de denker.
 */
void zet_tekenaar(teken_opdracht opdracht);
void standaard_tekenaar();

/*	BESTURING	*/

/**
 * Zet het scherm & glfw en opengl op.
 * Moet gebruikt worden voor andere dingen gedaan worden.
 */
void opzetten();

/**
 * Lust over de tekenopdracht & verwerkt invoer.
 * Stopt enkel zodra een vlag aangeeft dat het scherm moet sluiten.
 * Hierbij wordt alle opzet afgebroken.
 */
void lus();

/**
 * Tekent het HoekjeC logo.
 */
void logo(float tijd);

/**
 * Zet een vlag die aangeeft dat het scherm moet sluiten.
 */
void sluit_scherm();

#endif