#version 460 core

// Standaard Voorwerp
layout(location = 0) in vec3 plek;
layout(location = 1) in vec3 normaal;
layout(location = 2) in vec2 verfplek;

uniform dvec3 zicht_plek;
uniform mat4 zicht_matrix;
uniform mat4 voorwerp_matrix;

// Standaard Vorm
uniform uint soort;	 // kleur, afbeelding, materiaal.
uniform vec4 kleur;

// Snipper
flat out vec3 snipper_normaal;

void main() {
	if (soort == 0) {
		gl_Position = zicht_matrix * voorwerp_matrix * vec4(plek * 1.001, 1);
	} else {
		gl_Position = zicht_matrix * voorwerp_matrix * vec4(plek, 1);
	}
}
