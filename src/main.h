#ifndef MAIN_H
#define MAIN_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>

/**
 * @brief  Leest bestand naar string.
 * @param  *bestand_naam: de naam van het bestand.
 * @param  *bestand_string: de buffer waar naar geschreven moet wordt.
 * @param  bestand_grootte: grootte van de buffer.
 * @retval None
 */
void lees_bestand(const char* bestand_naam, char* bestand_string, size_t bestand_grootte);

void schermgrootte_terugroep(GLFWwindow* scherm, int breedte, int hoogte);

void toets_terugroep(GLFWwindow* scherm, int toets, int scancode, int handeling, int toevoeging);

void muisplek_terugroep(GLFWwindow* scherm, double x, double y);

void APIENTRY foutmelding_terugroep(GLenum bron, GLenum soort, unsigned int id, GLenum ernstigheid, GLsizei grootte,
									const char* bericht, const void* gebruikersParameter);

#endif