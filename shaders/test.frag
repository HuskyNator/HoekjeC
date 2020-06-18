# version 460 core

out vec4 kleur;

uniform mat4 testMat;

void main() {
	kleur = testMat * vec4(1, 0, 0, 1);
}