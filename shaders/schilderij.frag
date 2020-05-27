#version 460 core

in vec2 afbeelding_plek;

out vec4 kleur;

uniform sampler2D muur_afbeelding;
uniform sampler2D afbeelding;

void main(){
   vec4 muur = texture(muur_afbeelding, afbeelding_plek);
   vec4 beschildering = texture(afbeelding, afbeelding_plek);
   float duister = dot(beschildering.rgb, beschildering.rgb)/5;
   kleur = beschildering.a<0.05?muur:mix(muur, beschildering, 1-duister);
}
