#version 460 core

struct Materiaal {
	char* naam;
	vec3 vaste_kleur;					  // Licht onafhankelijk		(Ka)
	vec3 afweer_kleur;					  // Licht afhankelijk.			(Kd)
	vec3 weerkaats_kleur;				  // Lichtrichting afhankelijk.	(Ks)
	float weerkaatsing;					  // Mate van weerkaatsing.		(Ns)
	float doorzichtigheid;				  // Mate van doorzichtigheid.	(d)
	float brekingsgetal;				  // Mate van interne breking.	(Ni)
	/*unsigned*/ char verlichtingswijze;  // TODO mogelijk overbodig.	(illum)
}

layout(location = 0) in vec3 winkel_plek;
layout(location = 1) in vec2 verf_plek;
layout(location = 2) in vec3 winkel_normaal;
layout(location = 3) in vec4 winkel_kleur;

out flat vec4 kleur;

uniform mat4 teken_matrix;
uniform Materiaal materiaal;

void main() {
	gl_Position = teken_matrix * vec4(winkel_plek, 1);
	kleur = vec4(winkel_normaal, 1);
}