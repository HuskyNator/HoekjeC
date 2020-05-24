#version 460 core

in vec2 afbeelding_plek;

out vec4 kleur;

uniform sampler2D muur_afbeelding;
uniform sampler2D afbeelding;

void main(){
   vec4 muur = texture(muur_afbeelding, afbeelding_plek);
   vec4 ichigo = clamp(texture(afbeelding, afbeelding_plek)-0.1*vec4(1, 1, 1, 0), vec4(0,0,0,0), vec4(1,1,1,1));
   kleur = ichigo.a<0.05?muur:mix(muur, ichigo, 0.94);
}
