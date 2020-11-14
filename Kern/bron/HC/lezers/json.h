#ifndef JSON_H
#define JSON_H

#include "algemeen.h"
#include "lijsten/lijst.h"
#include "lijsten/sleutellijst.h"
#include "schrift.h"

typedef enum JSOORT JSOORT;
enum JSOORT { JNIKS, JFOUT, JNUL, JBOOLEAAN, JGETAL, JKOMMAGETAL, JSCHRIFT, JLIJST, JVOORWERP };

typedef SleutelLijst* JVoorwerp;  // JLid[Sleutel<Schrift>]
typedef Lijst* JLijst;			  // JLid[]

typedef union Jwaarde JWaarde;
union Jwaarde {
	JVoorwerp voorwerp;
	JLijst lijst;
	Schrift* letters;
	int getal;
	float kommagetal;
	booleaan booleaan;
};

typedef struct Jlid JLid;
struct Jlid {
	JSOORT soort;
	JWaarde waarde;
};

JLid leesJSON(FILE* bestand);

void verwijderJLid(JLid* JLid);
#endif