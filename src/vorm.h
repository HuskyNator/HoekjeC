#ifndef VORM_H
#define VORM_H

#include "lijst.h"
#include "lineair.h"

#include <stddef.h>

typedef struct vorm Vorm;
struct vorm {
	Lijst* hoeken;		// Vec3f[]
	Lijst* vlakken;		// Vlak[]
	Lijst* groepen;		// unsigned int[]
	Lijst* materialen;	// Materiaal[]

	unsigned int VAO;
	unsigned int* groep_EBOs;
};

typedef struct hoektallen Hoektallen;
struct hoektallen {
	unsigned int plek;
	unsigned int verf;
	unsigned int normaal;
};

typedef struct vlak Vlak;
struct vlak {
	unsigned int hoek1;
	unsigned int hoek2;
	unsigned int hoek3;
};

typedef struct vlakGroep VlakGroep;
struct vlakGroep{
	unsigned int grootte;
	unsigned int materiaal_tal;
};

/**
 * @param  hoeken: Vec3f[] van alle hoeken in de vorm.
 * @param  vlakken: Vlak[] van alle vlakken in de vorm.
 * @param  groepen: VlakGroep[] grootte van materiaalgroepen in de vlakken lijst.
 * @param  materialen: Materiaal[] van gebruikte materialen.
 */
Vorm* maakVorm(Lijst* hoeken, Lijst* vlakken, Lijst* groepen, Lijst* materialen);

/**
 * @brief  Voegt inhoud toe op de zoveelste standplaats van de voorwerp.
 * @param  vorm: de vorm om de inhoud aan toe te voegen.
 * @param  inhoud: de inhoud om toe te voegen.
 * @param  standplaats: de standplaats van de inhoud in de lijst van inhouden.
 * De standplaats mag niet overeen komen met al bezette standplaats (zoals 0).
 * Moet evenveel onderdelen hebben als het hoek_aantal van de vorm.
 */
void vormVoegInhoudToe(Vorm* vorm, const Lijst* inhoud, unsigned int standplaats);

void tekenVorm(const Vorm* vorm, Verver* verver);

void verwijderVorm(Vorm* vorm);

#endif