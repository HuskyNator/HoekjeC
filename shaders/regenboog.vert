#version 460 core

layout(location = 0) in vec3 plek;

out vec3 scherf_plek;

void main() {
    gl_Position = vec4(plek, 1.0);
    scherf_plek = plek;
}
