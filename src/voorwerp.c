
#include "voorwerp.h"

#include "GL/glew.h"
#include "lineair.h"
#include "verver.h"

static void werkMatrixBij(Voorwerp* voorwerp) {
	Mat4f matx = draaiMatrixx(voorwerp->draaix);
	Mat4f maty = draaiMatrixy(voorwerp->draaiy);
	Mat4f matz = draaiMatrixz(voorwerp->draaiz);
	Mat4f matPG = voorwerpMatrixPG(voorwerp->plaats, voorwerp->grootte);
	voorwerp->voorwerpMatrix =
		vermenigvuldigMatrix(matPG, vermenigvuldigMatrix(matx, vermenigvuldigMatrix(maty, matz)));
}

Voorwerp maakVoorwerp(Vorm* vorm, Vec3f plaats, Vec3f grootte, float draaix, float draaiy, float draaiz) {
	Voorwerp voorwerp = {vorm, plaats, grootte, draaix, draaiy, draaiz};
	werkMatrixBij(&voorwerp);
	return voorwerp;
}

void tekenVoorwerp(const Voorwerp* voorwerp, const Verver* verver) {
	zetVerverMat4f(verver, "voorwerpMatrix", &voorwerp->voorwerpMatrix);
	tekenVorm(voorwerp->vorm);
}
