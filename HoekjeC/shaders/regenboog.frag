#version 460 core
#define pi 3.14

in vec3 scherf_plek;

out vec4 scherf_kleur;

uniform float tijd;

void main() {
	float a = (scherf_plek.x + scherf_plek.y + 2)/4.0;
	float x = mod(a-tijd, 1);

	float r = x;
	float g = mod(x-1/3.0, 1);
	float b = mod(x+1/3.0, 1);

	float R = r>2.0/3.0?0:sin(3.0/2.0*pi*r);
	float G = g>2.0/3.0?0:sin(3.0/2.0*pi*g);
	float B = b>2.0/3.0?0:sin(3.0/2.0*pi*b);

	scherf_kleur = vec4(R, G, B, 1.0);
}
