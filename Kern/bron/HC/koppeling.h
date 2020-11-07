#ifndef KOPPELING_H
#define KOPPELING_H
#include "verf/verver.h"
#include "wiskunde/lineair.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

/**
 * @file
 * @short Koppeling tussen gebruik en HoekjeC.
 *
 * Bevat de kern van de koppeling tussen gebruik & HoekjeC (en zo eveneens opengl),
 * met verdere versimpelingen voor gemak, zoals muisbeweging, verplaatsing & een programmalus.
 */

/**		HOEKJEC		**/

char* krijg_kern_versie();

/**		SCHERM		**/

extern int schermbreedte;
extern int schermhoogte;
extern double schermverhouding;

void zet_schermnaam(const char* naam);
int krijg_schermbreedte();
int krijg_schermhoogte();
double krijg_schermverhouding();

/**		ZICHT MATRIXEN		**/

extern Mat4f plekM;		  // Tranfsormatiematrix van wereld naar plekrelatieve wereld. Verplaatst de wereld in plaats van het zicht.
extern Mat4f draai1M;	  // De eerste draaimatrix.
extern Mat4f draai2M;	  // De tweede draaimatrix.
extern Mat4f draai3M;	  // De derde draaimatrix.
extern Mat4f draaiM;	  // Transformatiematrix van plekrelatieve wereld naar relatieve wereld. Gelijk aan: draai3M * draai2M * draai1M.
extern Mat4f projectieM;  // Transformatiematrix van relatieve wereld naar zicht.
extern Mat4f zichtM;	  // Transformatiematrix van wereld naar zicht. Gelijk aan: projectieM * draaiM * plekM.

extern booleaan bijwerkbaar_plekM;		 // Of de matrix bij te werken is.
extern booleaan bijwerkbaar_draai1M;	 // Of de matrix bij te werken is.
extern booleaan bijwerkbaar_draai2M;	 // Of de matrix bij te werken is.
extern booleaan bijwerkbaar_draai3M;	 // Of de matrix bij te werken is.
extern booleaan bijwerkbaar_draaiM;		 // Of de matrix bij te werken is.
extern booleaan bijwerkbaar_projectieM;	 // Of de matrix bij te werken is.
extern booleaan bijwerkbaark_zichtM;	 // Of de matrix bij te werken is.

void werk_M_bij();	// Werkt de matrixen afzonderlijk bij, enkel als dit nodig is.

/*	PLEK	*/

/**
 * Deze getallen zijn niet bindend.
 * @param  plekx: Plek langs de x as.
 * @param  pleky: Plek langs de y as.
 * @param  plekz: Plek langs de z as.
 */
typedef Mat4f (*plek_maak_opdracht)(double plekx, double pleky, double plekz);
Mat4f standaard_plek_maker(double plekx, double pleky, double plekz);

extern Vec3d plek;

void zet_plek_maker(plek_maak_opdracht opdracht);
void zet_plekx(double nieuw);
void zet_pleky(double nieuw);
void zet_plekz(double nieuw);
void zet_plek(Vec3d nieuw);
void wijzig_plekx(double verandering);
void wijzig_pleky(double verandering);
void wijzig_plekz(double verandering);
void wijzig_plek(Vec3d verandering);

void werk_plekM_bij();

/*	DRAAI	*/

/**
 * Deze getallen zijn niet bindend.
 * @param  muisx: Horizontale muisplek.
 * @param  muisy : Verticale muisplek.
 */
typedef Mat4f (*draai_maak_opdracht)(double muisx, double muisy);

Mat4f standaard_draaix_maker(double muisx, double muisy);
Mat4f standaard_draaiy_maker(double muisx, double muisy);
Mat4f standaard_draaiz_maker(double muisx, double muisy);

void zet_draai1_maker(draai_maak_opdracht opdracht);
void zet_draai2_maker(draai_maak_opdracht opdracht);
void zet_draai3_maker(draai_maak_opdracht opdracht);

void werk_draaiM_bij();

/*	PROJECTIE	*/

/**
 * Deze getallen zijn niet bindend.
 * @param  voorvlak: Beginafstand van het zichtveld.
 * @param  achtervlak: Eindafstand van het zichtveld.
 * @param  zichthoek: Zichthoek van het zichtveld.
 * @param  schermverhouding: Verhouding van het zichtveld, breedte/hoogte.
 */
typedef Mat4f (*projectie_maak_opdracht)(float voorvlak, float achtervlak, float zichthoek, double schermverhouding);
Mat4f standaard_projectie_maker(float voorvlak, float achtervlak, float zichthoek, double schermverhouding);

extern float voorvlak;
extern float achtervlak;
extern float zichthoek;

void zet_projectie_maker(projectie_maak_opdracht opdracht);
void zet_voorvlak(float nieuw);
void zet_achtervlak(float nieuw);
void zet_zichthoek(float nieuw);

void werk_projectieM_bij();

/*	ZICHT	*/

void werk_zichtM_bij();

/**		MUIS		**/

/*	MUISPLEK	*/

extern double muisx;
extern double muisy;
extern float muisgevoeligheid;
extern double muisgrens;

void zet_muis(double x, double y);
void zet_muisgevoeligheid(float gevoeligheid);
void zet_muisgrens(double grens);

/*	MUISKNOP	*/

/**
 * @param  knop: De knop die is gebruikt.
 * @param  handeling: Wat er met de knop is gedaan.
 * @param  toevoeging: Binair aangifte van welke toevoegingen ingedrukt zijn (NUMLOCK, CAPSLOCK, SUPER, ALT, CONTROL, SHIFT).
 * @see GLFW_MOUSE_BUTTON_1 & GLFW_MOD_SHIFT
 */
typedef void (*muisknop_terugroep_opdracht)(int knop, int handeling, int toevoeging);
void standaard_muisknop_terugroeper(int knop, int handeling, int toevoeging);

void zet_muisknop_terugroeper(muisknop_terugroep_opdracht opdracht);

/**		TOETSEN		**/

/**
 * @param  toets: De toets die is gebruikt onder de aanname dat een VS toetsenbord gebruikt wordt.
 * @param  toets2: Een alternatief toetsgetal, welk altijd onveranderlijk en uniek is.
 * @param  handeling: Wat er met de toets is gedaan.
 * @param  toevoeging: Binaire aangifte van welke toevoegingen ingedrukt zijn (NUMLOCK, CAPSLOCK, SUPER, ALT, CONTROL, SHIFT).
 */
typedef void (*toets_terugroep_opdracht)(int toets, int toets2, int handeling, int toevoeging);
void standaard_toets_terugroeper(int toets, int toets2, int handeling, int toevoeging);

void zet_toets_terugroeper(toets_terugroep_opdracht opdracht);

/*	TPS	*/

extern double vorigeTijd;	  // Tijd van vorige teken oproep.
extern double tijdsverschil;  // Tijd tussen laatste en voorlopende teken oproepen.
double krijg_TPS();			  // Tekenoproepen per seconde.

/*	VERF	*/

// Krijg huidig gebruikte verver.
Verver krijg_huidige_verver();

/*	ACHTERGROND	*/

extern Vec4f achtergrond_kleur;

/**		TEKENEN		**/
/*	DENKER	*/

/**
 * Is verantwoordelijk voor het doen van denkstappen voor het programma.
 * Hier onder vallen verplaatsingen en voorwerp schepping/verwijdering.
 * Wordt opgeroepen voor (bijgewerkte) voorwerpen worden getekend.
 */
typedef void (*denk_opdracht)(double tijdsverschil);
void standaard_denker(double tijdsverschil);

void zet_denker(denk_opdracht opdracht);

/*	TEKENAAR	*/

/**
 * Is verantwoordelijk voor het tekenen van voorwerpen.
 * Het aanpassen van plekken / draaiïngen van dezen heeft mogelijk geen gevolg.
 * Wordt opgeroepen na oproeping van de denker.
 */
typedef void (*teken_opdracht)();
void standaard_tekenaar();

void zet_tekenaar(teken_opdracht opdracht);

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