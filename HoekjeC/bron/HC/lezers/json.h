#ifndef JSON_H
#define JSON_H

#include "HC/algemeen.h"
#include "HC/lijsten/lijst.h"
#include "HC/lijsten/sleutellijst.h"
#include "HC/schrift.h"

typedef enum JSOORT JSOORT;
enum JSOORT { JNIKS, JFOUT, JNULL, JBOOLEAAN, JGETAL, JKOMMAGETAL, JSCHRIFT, JLIJST, JVOORWERP };

typedef SleutelLijst* JVoorwerp;  // Json[Sleutel<Schrift>]
typedef Lijst* JLijst;			  // Json[]

typedef union Jwaarde JWaarde;
union Jwaarde {
	JVoorwerp voorwerp;
	JLijst lijst;
	Schrift* schrift;
	int getal;
	double kommagetal;
	booleaan booleaan;
};

typedef struct json Json;
struct json {
	JSOORT soort;
	JWaarde waarde;
};

Json leesJson(FILE* bestand);

void verwijderJson(Json* Json);
#endif