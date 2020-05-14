#version 460 core

layout (location = 0) in vec3 plek;
layout (location = 1) in vec3 kleur;

out vec3 hoekkleur;

void main(){
    gl_Position = vec4(plek, 1.0);
    hoekkleur = kleur;
}