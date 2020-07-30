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

layout(location = 0) in vec3 winkel_plek;
layout(location = 1) in vec2 verf_plek;
layout(location = 2) in vec3 winkel_normaal;

out vec3 plek;
out vec3 normaal;

uniform vec3 zicht_plek;
uniform mat4 zicht_matrix;
uniform mat4 voorwerp_matrix;
uniform Materiaal materiaal;

void main() {
	vec4 wereld_plek = voorwerp_matrix * vec4(winkel_plek, 1);
	gl_Position = zicht_matrix * wereld_plek;
	plek = wereld_plek.xyz / wereld_plek.w;
	normaal = winkel_normaal;
}