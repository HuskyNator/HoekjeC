#version 460 core

in vec2 afbeelding_plek;

out vec4 kleur;

uniform sampler2D afbeelding;

void main(){
   kleur = texture(muur_afbeelding, afbeelding_plek);
}
