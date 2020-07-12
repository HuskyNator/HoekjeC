
#include "voorwerp.h"

#include "GL/glew.h"
#include "lineair.h"
#include "koppeling.h"
#include "verver.h"

#include <stdlib.h>

struct voorwerp {
	Vorm* vorm;
	Vec3f plaats;
	Vec3f grootte;
	Vec3f draai;
	Mat4f voorwerpMatrix;
	Mat4f tekenMatrix;
};

static void werkVoorwerpMatrixBij(Voorwerp* voorwerp) {
	Mat4f matx = draaiMatrixx(voorwerp->draai.x);
	Mat4f maty = draaiMatrixy(voorwerp->draai.y);
	Mat4f matz = draaiMatrixz(voorwerp->draai.z);
	Mat4f matPG = voorwerpMatrixPG(voorwerp->plaats, voorwerp->grootte);
	voorwerp->voorwerpMatrix = Mat4fMat4f(matPG, Mat4fMat4f(matx, Mat4fMat4f(maty, matz)));
}

static void werkTekenMatrixBij(Voorwerp* voorwerp) {
	voorwerp->tekenMatrix = Mat4fMat4f(zichtMatrix, voorwerp->voorwerpMatrix);
}

Voorwerp* maakVoorwerp(Vorm* vorm, Vec3f plaats, Vec3f grootte, Vec3f draai) {
	Voorwerp* voorwerp = malloc(sizeof(Voorwerp));
	voorwerp->vorm = vorm;
	voorwerp->plaats = plaats;
	voorwerp->grootte = grootte;
	voorwerp->draai = draai;
	werkVoorwerpMatrixBij(voorwerp);
	werkTekenMatrixBij(voorwerp);
	return voorwerp;
}

void tekenVoorwerp(Voorwerp* voorwerp, const Verver* verver) {
	gebruikVerver(verver);
	if (zichtMatrixBijgewerkt) {
		werkTekenMatrixBij(voorwerp);
	}
	zetVerverMat4f(verver, "teken_matrix", &voorwerp->tekenMatrix);
	tekenVorm(voorwerp->vorm);
}
