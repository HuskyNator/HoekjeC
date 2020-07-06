#version 460 core

layout(location = 0) in vec3 winkel_plek;
layout(location = 1) in vec4 winkel_kleur;

out flat vec4 kleur;

uniform mat4 teken_matrix;
uniform vec4 voorwerp_kleur;

void main() {
	gl_Position = teken_matrix * vec4(winkel_plek, 1);
	kleur = winkel_kleur;
}