#version 460 core

layout(location = 0) in vec3 hoek_plek;
layout(location = 1) in vec2 hoek_afbeelding_plek;

out vec2 afbeelding_plek;

void main() {
	gl_Position = vec4(hoek_plek, 1.0);
	afbeelding_plek = hoek_afbeelding_plek;
}