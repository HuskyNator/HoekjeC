#version 460 core

in vec3 scherf_wereld_plek;
flat in vec3 scherf_normaal;
in vec2 scherf_verfplek;

out vec4 scherf_kleur;

uniform dvec3 zicht_plek;
uniform mat4 zicht_matrix;
uniform mat4 voorwerp_matrix;
uniform uint soort; // kleur, afbeelding, materiaal.
uniform vec4 kleur;

void main() {
	scherf_kleur = kleur;
}
