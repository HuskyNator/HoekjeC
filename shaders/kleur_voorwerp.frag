#version 460 core

struct Materiaal {
	// char* naam;
	vec3 vaste_kleur;		// Licht onafhankelijk		(Ka)
	vec3 afweer_kleur;		// Licht afhankelijk.			(Kd)
	vec3 weerkaats_kleur;	// Lichtrichting afhankelijk.	(Ks)
	float weerkaatsing;		// Mate van weerkaatsing.		(Ns)
	float doorzichtigheid;	// Mate van doorzichtigheid.	(d)
	float brekingsgetal;	// Mate van interne breking.	(Ni)

	// char verlichtingswijze;	// (illum)
};

in vec3 plek;
in vec3 normaal;

out vec4 fragment_kleur;

uniform vec3 zicht_plek;
uniform mat4 zicht_matrix;
uniform mat4 voorwerp_matrix;
uniform Materiaal materiaal;

void main() {
	vec3 n = normalize(normaal);
	vec3 zicht_r = normalize(zicht_plek - plek);
	vec3 licht_plek = vec3(2, 2, 2);
	vec3 licht_r = normalize(licht_plek - plek);

	vec3 afweer = materiaal.afweer_kleur * max(0, dot(n, licht_r));
	vec3 weerkaats = materiaal.weerkaats_kleur * max(0, pow(dot(zicht_r, reflect(-licht_r, n)), materiaal.weerkaatsing));
	fragment_kleur = vec4(1.0 / 2.0 * (afweer + weerkaats), 1);
}