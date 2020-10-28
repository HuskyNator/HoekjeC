#include "standaard_ververs.h"

#include "booleaan.h"
#include "verver.h"

#define KLEUR_VERVER_VERT "Kern/Ververs/Kleur.vert"
#define KLEUR_VERVER_FRAG "Kern/Ververs/Kleur.frag"
static unsigned int kleur_verver;
static booleaan kleur_verver_gemaakt = onwaar;

Verver krijgKleurVerver() {
	if (!kleur_verver_gemaakt) {
		kleur_verver = maakVerver(KLEUR_VERVER_VERT, KLEUR_VERVER_FRAG);
	}
	return kleur_verver;
}
