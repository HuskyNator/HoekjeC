#ifndef VORM_H
#define VORM_H

#include <stddef.h>

typedef struct vorm Vorm;

/**
 * @brief  Maakt een vorm.
 * @param  hoeken[]: Hoeken van de vorm.
 * @param  hoekaantal: Grootte van de hoeken in bytes.
 * @param  hoektallen[]: Hoektal volgorde van de vorm.
 * @param  hoektallengrootte: Grootte van hoektallen in bytes.
 */
Vorm* maakVorm(const float hoeken[], const size_t hoekengrootte, const unsigned int hoektallen[],
			   const size_t hoektallengrootte);

/**
 * @brief  Voegt inhoud toe op de zoveelste standplaats van de voorwerp.
 * @param  vorm: de vorm om de inhoud aan toe te voegen.
 * @param  inhoud[]: de inhoud om toe te voegen.
 * @param  inhoudsgrootte: grootte van de inhoud in bytes.
 * @param  onderdeelgrootte: de grootte van inhoudsonderdelen in floats.
 * @param  standplaats: de standplaats van de inhoud in de lijst van inhouden, mag niet overeen komen met al bezette
 * standplaats (zoals standplaats 0).
 */
void vormVoegInhoudToe(Vorm* vorm, const float inhoud[], size_t inhoudsgrootte, unsigned int onderdeelgrootte,
					   unsigned int standplaats);

void tekenVorm(const Vorm* vorm);

void verwijderVoorwerp(Vorm* vorm);

#endif