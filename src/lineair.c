#include "lineair.h"
#define _USE_MATH_DEFINES

#include <math.h>
#include <stdio.h>

// Inproducten

int inproduct2i(Vec2i a, Vec2i b) { return a.x * b.x + a.y * b.y; }
int inproduct3i(Vec3i a, Vec3i b) { return a.x * b.x + a.y * b.y + a.z * b.y; }
int inproduct4i(Vec4i a, Vec4i b) { return a.x * b.x + a.y * b.y + a.z * b.y + a.w * b.w; }

float inproduct2f(Vec2f a, Vec2f b) { return a.x * b.x + a.y * b.y; }
float inproduct3f(Vec3f a, Vec3f b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
float inproduct4f(Vec4f a, Vec4f b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }

// Uitproducten

Vec3i uitproducti(Vec3i a, Vec3i b) {
	return (Vec3i){a.y * b.z - b.y * a.z, a.z * b.x - b.z * a.x, a.x * b.y - b.x * a.y};
}
Vec3f uitproductf(Vec3f a, Vec3f b) {
	return (Vec3f){a.y * b.z - b.y * a.z, a.z * b.x - b.z * a.x, a.x * b.y - b.x * a.y};
}

// Overgangen

Vec4f Vec3n4f(Vec3f v, float w) { return (Vec4f){v.x, v.y, v.z, w}; }
Vec3f Vec4n3f(Vec4f v, booleaan normalizeren) {
	if (normalizeren) {
		return (Vec3f){v.x / v.w, v.y / v.w, v.z / v.w};
	} else {
		return (Vec3f){v.x, v.y, v.z};
	}
}
Mat4f kantelMat4f(Mat4f m) {
	return (Mat4f){{m.k1.x, m.k2.x, m.k3.x, m.k4.x},
				   {m.k1.y, m.k2.y, m.k3.y, m.k4.y},
				   {m.k1.z, m.k2.z, m.k3.z, m.k4.z},
				   {m.k1.w, m.k2.w, m.k3.w, m.k4.w}};
}

// Vermenigvuldigingen

Vec4f Vec4ff(Vec4f v, float f) { return (Vec4f){v.x * f, v.y * f, v.z * f, v.w * f}; }
Vec3f Vec3ff(Vec3f v, float f) { return (Vec3f){v.x * f, v.y * f, v.z * f}; }
Mat4f Mat4fMat4f(Mat4f L, Mat4f R) {
	return (Mat4f){{{L.k1.x * R.k1.x + L.k2.x * R.k1.y + L.k3.x * R.k1.z + L.k4.x * R.k1.w},
					{L.k1.y * R.k1.x + L.k2.y * R.k1.y + L.k3.y * R.k1.z + L.k4.y * R.k1.w},
					{L.k1.z * R.k1.x + L.k2.z * R.k1.y + L.k3.z * R.k1.z + L.k4.z * R.k1.w},
					{L.k1.w * R.k1.x + L.k2.w * R.k1.y + L.k3.w * R.k1.z + L.k4.w * R.k1.w}},
				   {{L.k1.x * R.k2.x + L.k2.x * R.k2.y + L.k3.x * R.k2.z + L.k4.x * R.k2.w},
					{L.k1.y * R.k2.x + L.k2.y * R.k2.y + L.k3.y * R.k2.z + L.k4.y * R.k2.w},
					{L.k1.z * R.k2.x + L.k2.z * R.k2.y + L.k3.z * R.k2.z + L.k4.z * R.k2.w},
					{L.k1.w * R.k2.x + L.k2.w * R.k2.y + L.k3.w * R.k2.z + L.k4.w * R.k2.w}},
				   {{L.k1.x * R.k3.x + L.k2.x * R.k3.y + L.k3.x * R.k3.z + L.k4.x * R.k3.w},
					{L.k1.y * R.k3.x + L.k2.y * R.k3.y + L.k3.y * R.k3.z + L.k4.y * R.k3.w},
					{L.k1.z * R.k3.x + L.k2.z * R.k3.y + L.k3.z * R.k3.z + L.k4.z * R.k3.w},
					{L.k1.w * R.k3.x + L.k2.w * R.k3.y + L.k3.w * R.k3.z + L.k4.w * R.k3.w}},
				   {{L.k1.x * R.k4.x + L.k2.x * R.k4.y + L.k3.x * R.k4.z + L.k4.x * R.k4.w},
					{L.k1.y * R.k4.x + L.k2.y * R.k4.y + L.k3.y * R.k4.z + L.k4.y * R.k4.w},
					{L.k1.z * R.k4.x + L.k2.z * R.k4.y + L.k3.z * R.k4.z + L.k4.z * R.k4.w},
					{L.k1.w * R.k4.x + L.k2.w * R.k4.y + L.k3.w * R.k4.z + L.k4.w * R.k4.w}}};
}

Vec4f Mat4fVec4f(Mat4f L, Vec4f R) {
	return (Vec4f){L.k1.x * R.x + L.k2.x * R.y + L.k3.x * R.z + L.k4.x * R.w,
				   L.k1.y * R.x + L.k2.y * R.y + L.k3.y * R.z + L.k4.y * R.w,
				   L.k1.z * R.x + L.k2.z * R.y + L.k3.z * R.z + L.k4.z * R.w,
				   L.k1.w * R.x + L.k2.w * R.y + L.k3.w * R.z + L.k4.w * R.w};
}

// Draaiingen

Mat4f draaiMatrixx(float draai) {
	float cos = cosf(draai);
	float sin = sinf(draai);
	return (Mat4f){{1, 0, 0, 0}, {0, cos, sin, 0}, {0, -sin, cos, 0}, {0, 0, 0, 1}};
	// Draai X:
	// |1			0			0			0|
	// |0			cos(d)		-sin(d)		0|
	// |0			sin(d)		cos(d)		0|
	// |0			0			0			1|
}

Mat4f draaiMatrixy(float draai) {
	float cos = cosf(draai);
	float sin = sinf(draai);
	return (Mat4f){{cos, 0, -sin, 0}, {0, 1, 0, 0}, {sin, 0, cos, 0}, {0, 0, 0, 1}};
	// Draai Y:
	// |cos(d)		0			sin(d)		0|
	// |0			1			0			0|
	// |-sin(d)		0			cos(d)		0|
	// |0			0			0			1|
}

Mat4f draaiMatrixz(float draai) {
	float cos = cosf(draai);
	float sin = sinf(draai);
	return (Mat4f){{cos, sin, 0, 0}, {-sin, cos, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
	// Draai Z:
	// |cos(d)		-sin(d)		0			0|
	// |sin(d)		cos(d)		0			0|
	// |0			0			1			0|
	// |0			0			0			1|
}

// Matrixen

Mat4f identiteitsMatrix() { return (Mat4f){{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}; }
Mat4f verplaatsMatrix(float x, float y, float z) {
	return (Mat4f){{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {x, y, z, 1}};
}
Mat4f perspectiefMatrix(float voorvlak, float achtervlak, double zichthoek, float schermverhouding) {
	float invtan = 1 / tan(M_PI / 180.0 * zichthoek / 2.0);
	float invdelta = 1 / (achtervlak - voorvlak);
	return (Mat4f){{invtan, 0, 0, 0},
				   {0, schermverhouding * invtan, 0, 0},
				   {0, 0, (voorvlak + achtervlak) * invdelta, 1},
				   {0, 0, -2 * voorvlak * achtervlak * invdelta}};
	// Mat Projec:
	// |1/tan(a/2)	0			0			0			|
	// |0			R/tan(a/2)	0			0			|
	// |0			0			(A+V)/(A-V)	-2AV/(A-V)	|
	// |0			0			1			0			|
}
Mat4f voorwerpMatrixPG(Vec3f P, Vec3f G) {
	return (Mat4f){{G.x, 0, 0, 0}, {0, G.y, 0, 0}, {0, 0, G.z, 0}, {P.x, P.y, P.z, 1}};
}

// Printen

void printVec4f(Vec4f v) { printf("[%f, %f, %f, %f]\n", v.x, v.y, v.z, v.w); }
void printVec3f(Vec3f v) { printf("[%f, %f, %f]\n", v.x, v.y, v.z); }
void printMat4f(Mat4f m) {
	Mat4f gekanteld = kantelMat4f(m);
	putchar('{');
	putchar('\n');
	printVec4f(gekanteld.k1);
	printVec4f(gekanteld.k2);
	printVec4f(gekanteld.k3);
	printVec4f(gekanteld.k4);
	putchar('}');
}
