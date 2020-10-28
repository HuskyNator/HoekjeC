#version 460 core

layout(location = 0) in vec3 plek;
layout(location = 1) in vec3 normaal;
layout(location = 2) in vec2 verfplek;

out vec3 scherf_wereld_plek;
flat out vec3 scherf_normaal;
out vec2 scherf_verfplek;

uniform dvec3 zicht_plek;
uniform mat4 zicht_matrix;
uniform mat4 voorwerp_matrix;
uniform uint soort; // kleur, afbeelding, materiaal.
uniform vec4 kleur;

void main() {
	vec4 wereld_plek = voorwerp_matrix * vec4(plek, 1);
	gl_Position = zicht_matrix * wereld_plek;

	scherf_wereld_plek = wereld_plek.xyz / wereld_plek.w;
	scherf_normaal = normaal;
	scherf_verfplek = verfplek;
}
