#ifndef VORM_H
#define VORM_H

#include "lineair.h"

#include <stddef.h>

typedef struct vorm Vorm;
struct vorm {
	unsigned int hoek_aantal;
	unsigned int vlak_aantal;
	unsigned int VAO;
};

typedef struct hoek Hoek;
static struct hoek{
	Vec3f hoek;
	Vec3f verf;
	Vec3f normaal;
};
//TODO maak "struct hoek" aan voor {hoek, verf, normaal}.

typedef struct hoektal Hoektal;
static struct hoektal {
	Vec3ui hoek;
	Vec3ui verf;
	Vec3ui normaal;
};

/**
 * @brief  Maakt een vorm.
 * @param  hoeken[]: Hoeken van de vorm.
 * @param  hoekaantal: Grootte van de hoeken in bytes.
 * @param  hoektallen[]: Hoektal volgorde van de vorm.
 * @param  hoektallengrootte: Grootte van hoektallen in bytes.
 */
Vorm* maakVorm(const Vec3f* hoeken, const size_t hoekengrootte, const Vec3ui* hoektallen,
			   const size_t hoektallengrootte);

/**
 * @brief  Voegt inhoud toe op de zoveelste standplaats van de voorwerp.
 * @param  vorm: de vorm om de inhoud aan toe te voegen.
 * @param  inhoud[]: de inhoud om toe te voegen.
 * @param  inhoudsgrootte: grootte van de inhoud in bytes.
 * @param  onderdeelgrootte: de grootte van inhoudsonderdelen in floats.
 * @param  standplaats: de standplaats van de inhoud in de lijst van inhouden.
 * De standplaats mag niet overeen komen met al bezette standplaats (zoals 0).
 * Moet evenveel onderdelen hebben als het hoek_aantal van de vorm.
 */
void vormVoegInhoudToe(Vorm* vorm, const float inhoud[], size_t inhoudsgrootte, unsigned int onderdeelgrootte,
					   unsigned int standplaats);

void tekenVorm(const Vorm* vorm);

void verwijderVorm(Vorm* vorm);

#endif