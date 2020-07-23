#ifndef MATERIAAL_H
#define MATERIAAL_H

typedef struct materiaal Materiaal;

struct materiaal {
	char* naam;
	Vec3f vaste_kleur;				  // Licht onafhankelijk		(Ka)
	Vec3f afweer_kleur;				  // Licht afhankelijk.			(Kd)
	Vec3f weerkaats_kleur;			  // Lichtrichting afhankelijk.	(Ks)
	float weerkaatsing;				  // Mate van weerkaatsing.		(Ns)
	float doorzichtigheid;			  // Mate van doorzichtigheid.	(d)
	float brekingsgetal;			  // Mate van interne breking.	(Ni)
	unsigned char verlichtingswijze;  // TODO mogelijk overbodig.	(illum)
};

#endif