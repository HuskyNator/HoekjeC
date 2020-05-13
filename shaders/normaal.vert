#version 460 core

layout(location = 0) in vec3 plek;

out vec4 winkel_kleur;

void main() {
    gl_Position = vec4(plek, 1.0);
    winkel_kleur = vec4(0.5+plek.x/2, 0.5+plek.y/2, 0, 1.0);
}
