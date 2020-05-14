#version 460 core

in vec3 hoekkleur;

out vec4 scherfkleur;

void main() {
    scherfkleur = vec4(hoekkleur, 1.0);
}