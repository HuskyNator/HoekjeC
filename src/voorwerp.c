
#include "voorwerp.h"

#include "GL/glew.h"
#include "koppeling.h"
#include "lineair.h"
#include "verver.h"

#include <stdlib.h>

struct voorwerp {
	Vorm* vorm;
	Vec3f plaats;
	Vec3f grootte;
	Vec3f draai;
	Mat4f voorwerpMatrix;
};

static void werkVoorwerpMatrixBij(Voorwerp* voorwerp) {
	Mat4f matx = draaiMatrixx(voorwerp->draai.x);
	Mat4f maty = draaiMatrixy(voorwerp->draai.y);
	Mat4f matz = draaiMatrixz(voorwerp->draai.z);
	Mat4f matPG = voorwerpMatrixPlekGrootte(voorwerp->plaats, voorwerp->grootte);
	voorwerp->voorwerpMatrix = Mat4fMat4f(matPG, Mat4fMat4f(matx, Mat4fMat4f(maty, matz)));
}

Voorwerp* maakVoorwerp(Vorm* vorm, Vec3f plaats, Vec3f grootte, Vec3f draai) {
	Voorwerp* voorwerp = malloc(sizeof(Voorwerp));
	voorwerp->vorm = vorm;
	voorwerp->plaats = plaats;
	voorwerp->grootte = grootte;
	voorwerp->draai = draai;
	werkVoorwerpMatrixBij(voorwerp);
	return voorwerp;
}

void tekenVoorwerp(Voorwerp* voorwerp, Verver* verver) {
	gebruikVerver(verver);
	Vec3f plek = Vec3dn3f(krijg_plek());
	zetVerverFloat3v(verver, "zicht_plek", &plek.x);
	zetVerverMat4f(verver, "zicht_matrix", &zichtM);
	zetVerverMat4f(verver, "voorwerp_matrix", &voorwerp->voorwerpMatrix);
	tekenVorm(voorwerp->vorm, verver);
}
