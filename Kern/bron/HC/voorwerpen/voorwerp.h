#ifndef VOORWERP2_H
#define VOORWERP2_H

#include "HC/booleaan.h"
#include "HC/lijsten/lijst.h"
#include "HC/verf/verver.h"
#include "HC/wiskunde/lineair.h"
#include "voowerp_gegevens.h"

/**
 * @file
 * @short Voorwerp bedoeld voor tekenen.
 */
typedef struct voorwerp Voorwerp;
struct voorwerp {
	const struct voorwerp_opdrachten* opdrachten;
	union {
		void* gegevens;
		VormGegevens* vormgegevens;
		Lijst* groepkinderen;
	};

	// TODO verplaatsen naar gegevens.
	unsigned int VAO;
	unsigned int VBOs[3];
	unsigned int EBO;
	unsigned int EBO_aantal;

	Vec3f plek;
	Vec3f grootte;
	Vec3f draaiing;
	Mat4f voorwerpM;
	booleaan bijgewerkt;

	Voorwerp* ouder;
	// Verwijst naar de voorwerpM als er geen ouder is.
	// Anders is het het resultaat van degene van de ouderr & de eigen matrix.f
	Mat4f* verzamelM;
};

struct voorwerp_opdrachten {  // TODO: toch terugzetten in voorwerp?
	void (*teken_opdracht)(Voorwerp* voorwerp, Verver verver);
	void (*verwijder_opdracht)(Voorwerp* voorwerp);
};

/**
 * @brief Maakt een leeg voorwerp met beginwaarden.
 */
Voorwerp* maakVoorwerp();

/**
 * @brief  Maakt een voorwerp bruikbaar door OpenGL.
 *
 * @param[out]  VAO: Vertex array object nummer.
 * @param[out]  VBOs: Vertex buffer object nummers, voor de 3 plek, normaal & verf buffers.
 * @param[out]  EBO: Element buffer object nummer, voor de tallen buffer, welk de tekenvolgorde van hoekjes aangeeft.
 * @param plekken: Plekken van de hoekjes. (Vec3f)
 * @param  normalen: Normalen van de hoekjes. (Vec3f)
 * @param  verfplekken: Verfplekken van de hoekjes. (Vec2f)
 * @param  tekentallen: Tekentallen, geeft aan op welke volgorde de hoekjes getekend moeten worden.
 * @param  hoek_aantal: Aantal hoekjes.
 * @param  tekental_aantal: Aantal tekentallen.
 * @retval None
 */
void maak_voorwerp_bruikbaar(unsigned int* VAO, unsigned int* VBOs, unsigned int* EBO, const Vec3f* plekken, const Vec3f* normalen,
				   const Vec2f* verfplekken, const unsigned int* tekentallen, unsigned int hoek_aantal, unsigned int tekental_aantal);

void voorwerpZetPlek(Voorwerp* voorwerp, Vec3f plek);
void voorwerpZetGrootte(Voorwerp* voorwerp, Vec3f grootte);
void voorwerpZetDraai(Voorwerp* voorwerp, Vec3f draai);

void voorwerpZetM(Voorwerp* vorm);

void voorwerpTeken(Voorwerp* voorwerp, Verver verver);

void verwijderVoorwerp(Voorwerp* voorwerp);

#endif