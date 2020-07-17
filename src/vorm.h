#ifndef VORM_H
#define VORM_H

#include "lijst.h"
#include "lineair.h"

#include <stddef.h>

typedef struct vorm Vorm;
struct vorm {
	unsigned int hoek_aantal;
	unsigned int vlak_aantal;
	unsigned int VAO;
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

/**
 * @brief  Maakt een vorm.
 * @param  hoeken: Hoek lijst van de vorm, bevat plekken van elke hoek.
 * @param  vlakken: Vlak lijst van de vorm, bevat verwijzingen naar hoeken.
 */
Vorm* maakVorm(const Lijst* hoeken, const Lijst* vlakken);

/**
 * @brief  Voegt inhoud toe op de zoveelste standplaats van de voorwerp.
 * @param  vorm: de vorm om de inhoud aan toe te voegen.
 * @param  inhoud: de inhoud om toe te voegen.
 * @param  standplaats: de standplaats van de inhoud in de lijst van inhouden.
 * De standplaats mag niet overeen komen met al bezette standplaats (zoals 0).
 * Moet evenveel onderdelen hebben als het hoek_aantal van de vorm.
 */
void vormVoegInhoudToe(Vorm* vorm, const Lijst* inhoud, unsigned int standplaats);

void tekenVorm(const Vorm* vorm);

void verwijderVorm(Vorm* vorm);

#endif