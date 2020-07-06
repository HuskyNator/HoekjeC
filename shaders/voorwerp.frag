#version 460 core

in flat vec4 kleur;

out vec4 fragment_kleur;

void main() { fragment_kleur = kleur; }