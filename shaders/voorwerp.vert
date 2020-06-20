#version 460 core

layout(location = 0) in vec3 winkel_plek;

uniform mat4 projectie_matrix;
uniform mat4 zicht_matrix;
uniform mat4 voorwerp_matrix;

void main() { gl_Position = projectie_matrix * zicht_matrix * voorwerp_matrix * vec4(winkel_plek, 1); }