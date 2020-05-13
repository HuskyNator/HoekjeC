#ifndef MAIN_H
#define MAIN_H

void lees_bestand(char *bestandsnaam, char* bestand_string);
void schermgrootte_terugroep(GLFWwindow* scherm, int breedte, int hoogte);
void toets_terugroep(GLFWwindow *scherm, int toets, int scancode, int handeling, int toevoeging);

#endif