#include "lineair.h"

#include "math.h"

// Inproducten.
bool inproduct2b(Vec2b a, Vec2b b) { return a.x && b.x || a.y && b.y; }
bool inproduct3b(Vec3b a, Vec3b b) { return a.x && b.x || a.y && b.y || a.z && b.z; }
bool inproduct4b(Vec4b a, Vec4b b) { return a.x && b.x || a.y && b.y || a.z && b.z || a.w && b.w; }

int inproduct2i(Vec2i a, Vec2i b) { return a.x * b.x + a.y * b.y; }
int inproduct3i(Vec3i a, Vec3i b) { return a.x * b.x + a.y * b.y + a.z * b.y; }
int inproduct4i(Vec4i a, Vec4i b) { return a.x * b.x + a.y * b.y + a.z * b.y + a.w * b.w; }

float inproduct2f(Vec2f a, Vec2f b) { return a.x * b.x + a.y * b.y; }
float inproduct3f(Vec3f a, Vec3f b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
float inproduct4f(Vec4f a, Vec4f b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }

// Uitproducten.
Vec3b uitproductb(Vec3b a, Vec3b b) {
	return (Vec3b){a.y && b.z || !(b.y && a.z), a.z && b.x || !(b.z * a.x), a.x && b.y || !(b.x * a.y)};
}

Vec3i uitproducti(Vec3i a, Vec3i b) {
	return (Vec3i){a.y * b.z - b.y * a.z, a.z * b.x - b.z * a.x, a.x * b.y - b.x * a.y};
}

Vec3f uitproductf(Vec3f a, Vec3f b) {
	return (Vec3f){a.y * b.z - b.y * a.z, a.z * b.x - b.z * a.x, a.x * b.y - b.x * a.y};
}

// Matrix Toepassingen.
Mat4f identiteitsMatrix() { return (Mat4f){{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}; }

Mat4f vermenigvuldigMatrix(Mat4f L, Mat4f R) {
	return (Mat4f){{L.r1.x * R.r1.x + L.r1.y * R.r2.x + L.r1.z * R.r3.x + L.r1.w * R.r4.x,
					L.r1.x * R.r1.y + L.r1.y * R.r2.y + L.r1.z * R.r3.y + L.r1.w * R.r4.y,
					L.r1.x * R.r1.z + L.r1.y * R.r2.z + L.r1.z * R.r3.z + L.r1.w * R.r4.z,
					L.r1.x * R.r1.w + L.r1.y * R.r2.w + L.r1.z * R.r3.w + L.r1.w * R.r4.w},
				   {L.r2.x * R.r1.x + L.r2.y * R.r2.x + L.r2.z * R.r3.x + L.r2.w * R.r4.x,
					L.r2.x * R.r1.y + L.r2.y * R.r2.y + L.r2.z * R.r3.y + L.r2.w * R.r4.y,
					L.r2.x * R.r1.z + L.r2.y * R.r2.z + L.r2.z * R.r3.z + L.r2.w * R.r4.z,
					L.r2.x * R.r1.w + L.r2.y * R.r2.w + L.r2.z * R.r3.w + L.r2.w * R.r4.w},
				   {L.r3.x * R.r1.x + L.r3.y * R.r2.x + L.r3.z * R.r3.x + L.r3.w * R.r4.x,
					L.r3.x * R.r1.y + L.r3.y * R.r2.y + L.r3.z * R.r3.y + L.r3.w * R.r4.y,
					L.r3.x * R.r1.z + L.r3.y * R.r2.z + L.r3.z * R.r3.z + L.r3.w * R.r4.z,
					L.r3.x * R.r1.w + L.r3.y * R.r2.w + L.r3.z * R.r3.w + L.r3.w * R.r4.w},
				   {L.r4.x * R.r1.x + L.r4.y * R.r2.x + L.r4.z * R.r3.x + L.r4.w * R.r4.x,
					L.r4.x * R.r1.y + L.r4.y * R.r2.y + L.r4.z * R.r3.y + L.r4.w * R.r4.y,
					L.r4.x * R.r1.z + L.r4.y * R.r2.z + L.r4.z * R.r3.z + L.r4.w * R.r4.z,
					L.r4.x * R.r1.w + L.r4.y * R.r2.w + L.r4.z * R.r3.w + L.r4.w * R.r4.w}};
}

Mat4f verplaatsMatrix(float x, float y, float z) { return (Mat4f){{1, 0, 0, x}, {0, 1, 0, y}, {0, 0, 1, z}, {0, 0, 0, 1}}; }

Mat4f draaiMatrixx(float draai) {
	return (Mat4f){{1, 0, 0, 0}, {0, cosf(draai), -sinf(draai), 0}, {0, sinf(draai), cosf(draai), 0}, {0, 0, 0, 1}};
	// Draai X:
	// |1			0			0			0|
	// |0			cos(d)		-sin(d)		0|
	// |0			sin(d)		cos(d)		0|
	// |0			0			0			1|
}

Mat4f draaiMatrixy(float draai) {
	return (Mat4f){{cosf(draai), 0, sinf(draai), 0}, {0, 1, 0, 0}, {-sinf(draai), 0, cosf(draai), 0}, {0, 0, 0, 1}};
	// Draai Y:
	// |cos(d)		0			sin(d)		0|
	// |0			1			0			0|
	// |-sin(d)		0			cos(d)		0|
	// |0			0			0			1|
}

Mat4f draaiMatrixz(float draai) {
	return (Mat4f){{cosf(draai), -sinf(draai), 0, 0}, {sinf(draai), cosf(draai), 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
	// Draai Z:
	// |cos(d)		-sin(d)		0			0|
	// |sin(d)		cos(d)		0			0|
	// |0			0			1			0|
	// |0			0			0			1|
}

Mat4f perspectiefMatrix(float voorvlak, float achtervlak, double zichthoek, float schermverhouding) {
	float zichthoek_verhouding = 1 / tan(zichthoek / 2);
	float delta = 1 / (achtervlak - voorvlak);
	return (Mat4f){{zichthoek_verhouding, 0, 0, 0},
				   {0, schermverhouding * zichthoek_verhouding, 0, 0},
				   {0, 0, (achtervlak + voorvlak) * delta, -2 * achtervlak * voorvlak * delta},
				   {0, 0, 1, 0}};
	// Mat Projec:
	// |1/tan(a/2)	0			0			0			|
	// |0			R/tan(a/2)	0			0			|
	// |0			0			A+V/(A-V)	-2AV/(A-V)	|
	// |0			0			1			0			|
}

Mat4f voorwerpMatrixPG(Vec3f P, Vec3f G) {
	return (Mat4f){{G.x, 0, 0, P.x}, {0, G.y, 0, P.y}, {0, 0, G.z, P.z}, {0, 0, 0, 1}};
}
