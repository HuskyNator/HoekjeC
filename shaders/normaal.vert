# version 460 core

layout (location = 0) in vec3 winkel_plek;

void main() {
    gl_Position = vec4(winkel_plek, 1.0);
}