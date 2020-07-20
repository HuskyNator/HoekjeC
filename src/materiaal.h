#ifndef MATERIAAL_H
#define MATERIAAL_H

typedef struct materiaal Materiaal;

struct materiaal {
	char* naam;
	Vec3f vaste_kleur;				  // Licht onafhankelijk
	Vec3f afweer_kleur;				  // Licht afhankelijk.
	Vec3f weerkaats_kleur;			  // Lichtrichting afhankelijk.
	float weerkaatsing;				  // Mate van weerkaatsing.
	float doorzichtigheid;			  // Mate van doorzichtigheid.
	float brekingsgetal;			  // Mate van interne breking.
	unsigned char verlichtingswijze;  // TODO mogelijk overbodig.
};

#endif