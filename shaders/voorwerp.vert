#version 460 core

layout(location = 0) in vec3 winkel_plek;

uniform mat4 teken_matrix;

void main() { gl_Position = teken_matrix * vec4(winkel_plek, 1); }