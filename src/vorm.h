#ifndef VORM_H
#define VORM_H

#include "lijst.h"
#include "lineair.h"
#include "materiaal.h"
#include "verver.h"

#include <stddef.h>

typedef struct vorm Vorm;
struct vorm {
	unsigned int hoek_aantal;
	Lijst* hoeken_p;	// Vec3f[]
	Lijst* hoeken_v;	// Vec2f[]
	Lijst* hoeken_n;	// Vec3f[]
	Lijst* vlakken;		// Vlak[]
	Lijst* groepen;		// VlakGroep[]
	Lijst* materialen;	// Materiaal[]

	unsigned int VAO;
	unsigned int* groep_EBOs;
};

typedef struct vlak Vlak;
struct vlak {
	unsigned int hoek1;
	unsigned int hoek2;
	unsigned int hoek3;
};

typedef struct vlakGroep VlakGroep;
struct vlakGroep {
	unsigned int grootte;
	unsigned int materiaal_tal;
};

/**
 * @param  hoeken_p: Vec3f[] plekken van alle hoeken in de vorm.
 * @param  hoeken_v: Vec2f[] verf plekken van alle hoeken in de vorm.
 * @param  hoeken_n: Vec3f[] normalen van alle hoeken in de vorm.
 * @param  vlakken: Vlak[] van alle vlakken in de vorm.
 * @param  groepen: VlakGroep[] grootte van materiaalgroepen in de vlakken lijst.
 * @param  materialen: Materiaal[] van gebruikte materialen.
 */
Vorm* maakVorm(Lijst* hoeken_p, Lijst* hoeken_v, Lijst* hoeken_n, Lijst* vlakken, Lijst* groepen, Lijst* materialen);

/**
 * @brief  Voegt inhoud toe op de zoveelste standplaats van de voorwerp.
 * @param  vorm: de vorm om de inhoud aan toe te voegen.
 * @param  inhoud: de inhoud om toe te voegen.
 * @param  standplaats: de standplaats van de inhoud in de lijst van inhouden.
 * De standplaats mag niet overeen komen met al bezette standplaats, zoals 0, 1 & 2 (p, v & n).
 * De inhoud moet evenveel onderdelen bevatten als het aantal hoeken.
 */
void vormVoegInhoudToe(Vorm* vorm, const Lijst* inhoud, unsigned int standplaats);

void tekenVorm(const Vorm* vorm, Verver* verver);

void verwijderVorm(Vorm* vorm);

#endif