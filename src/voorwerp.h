#ifndef VOORWERP_H
#define VOORWERP_H

typedef struct Voorwerp Voorwerp;

struct Voorwerp
{
    unsigned int grootte;
    unsigned int VAO;
    // unsigned int EBO;
    //unsigned int VBOs[];
};

/**
 * @brief  Maakt een voorwerp.
 * @param  hoeken[]: Hoeken van het voorwerp.
 * @param  hoekaantal: Grootte van de hoeken in bytes.
 * @param  hoektallen[]: Hoektal volgorde van het voorwerp.
 * @param  hoektallengrootte: Grootte van hoektallen in bytes.
 */
Voorwerp *maakVoorwerp(const float hoeken[], const size_t hoekengrootte, const unsigned int hoektallen[], const size_t hoektallengrootte);

/**
 * @brief  Voegt inhoud toe op de zoveelste standplaats van het voorwerp.
 * @param  voorwerp: het voorwerp om de inhoud aan toe te voegen.
 * @param  inhoud[]: de inhoud om toe te voegen.
 * @param  inhoudsgrootte: grootte van de inhoud in bytes.
 * @param  onderdeelgrootte: de grootte van inhoudsonderdelen in floats.
 * @param  standplaats: de standplaats van de inhoud in de lijst van inhouden, mag niet overeen komen met al bezette standplaats (zoals standplaats 0).
 */
void voorwerpVoegToe(Voorwerp *voorwerp, const float inhoud[], size_t inhoudsgrootte, unsigned int onderdeelgrootte, unsigned int standplaats);

/**
 * Teken het voorwerp.
 */
void tekenVoorwerp(const Voorwerp *voorwerp);

void verwijderVoorwerp(Voorwerp *voorwerp);

#endif