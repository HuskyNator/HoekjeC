#version 460 core

out vec4 scherf_kleur;

uniform vec4 voorwerp_kleur;

void main() { scherf_kleur = voorwerp_kleur; }