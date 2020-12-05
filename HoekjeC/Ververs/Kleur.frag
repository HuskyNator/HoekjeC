#version 460 core

out vec4 snipperkleur;

// Standaard Voorwerp

uniform dvec3 zicht_plek;
uniform mat4 zicht_matrix;
uniform mat4 voorwerp_matrix;

// Standaard Vorm
uniform uint soort;	 // kleur, afbeelding, materiaal.
uniform vec4 kleur;

// Snipper
flat in vec3 snipper_normaal;

void main() {
	snipperkleur= kleur;
}
