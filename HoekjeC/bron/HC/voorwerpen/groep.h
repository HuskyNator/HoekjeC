#ifndef GROEP_H
#define GROEP_H

#include "HC/algemeen.h"
#include "voorwerp.h"

/**
 * @file
 * @short Groep voorwerpen die samen getekend worden.
 *
 * Houdt een lijst bij van voorwerpen om te tekenen.
 * Voorwerpen worden getekend door de groep matrix te vermenigvuldigen
 * met de matrix des voorwerps.
 *
 * @param verwijder_kinderen Of kinderen van de groep verwijderd moeten worden wanneer de groep verwijderd wordt.
 */
typedef Voorwerp Groep;

void groep_teken_opdracht(Groep* groep, Verver verver);
void groep_verwijder_opdracht(Groep* groep);
static const struct voorwerp_opdrachten groep_opdrachten = {.teken_opdracht = groep_teken_opdracht,
															.verwijder_opdracht = groep_verwijder_opdracht};

Groep* maakGroep(booleaan verwijder_kinderen);

/**
 * @brief Voegt een voorwerp toe aan de groep.
 */
void groepVoeg(Groep* groep, Voorwerp* voorwerp);

/**
 * @brief Verwijder een voorwerp uit de groep.
 */
void groepVerwijder(Groep* groep, Voorwerp* voorwerp);

#endif