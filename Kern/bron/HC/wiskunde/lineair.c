#include "lineair.h"
#define _USE_MATH_DEFINES

#include <math.h>
#include <stdio.h>

/**		VECTOREN		**/

/*	PLUS	*/

Vec2i Vec2iP(Vec2i een, Vec2i twee) { return (Vec2i){een.x + twee.x, een.y + twee.y}; }
Vec3i Vec3iP(Vec3i een, Vec3i twee) { return (Vec3i){een.x + twee.x, een.y + twee.y, een.z + twee.z}; }
Vec4i Vec4iP(Vec4i een, Vec4i twee) { return (Vec4i){een.x + twee.x, een.y + twee.y, een.z + twee.z, een.w + twee.w}; }
Vec2u Vec2uP(Vec2u een, Vec2u twee) { return (Vec2u){een.x + twee.x, een.y + twee.y}; }
Vec3u Vec3uP(Vec3u een, Vec3u twee) { return (Vec3u){een.x + twee.x, een.y + twee.y, een.z + twee.z}; }
Vec4u Vec4uP(Vec4u een, Vec4u twee) { return (Vec4u){een.x + twee.x, een.y + twee.y, een.z + twee.z, een.w + twee.w}; }
Vec2f Vec2fP(Vec2f een, Vec2f twee) { return (Vec2f){een.x + twee.x, een.y + twee.y}; }
Vec3f Vec3fP(Vec3f een, Vec3f twee) { return (Vec3f){een.x + twee.x, een.y + twee.y, een.z + twee.z}; }
Vec4f Vec4fP(Vec4f een, Vec4f twee) { return (Vec4f){een.x + twee.x, een.y + twee.y, een.z + twee.z, een.w + twee.w}; }
Vec2d Vec2dP(Vec2d een, Vec2d twee) { return (Vec2d){een.x + twee.x, een.y + twee.y}; }
Vec3d Vec3dP(Vec3d een, Vec3d twee) { return (Vec3d){een.x + twee.x, een.y + twee.y, een.z + twee.z}; }
Vec4d Vec4dP(Vec4d een, Vec4d twee) { return (Vec4d){een.x + twee.x, een.y + twee.y, een.z + twee.z, een.w + twee.w}; }

/*	MIN	*/

Vec2i Vec2iM(Vec2i een, Vec2i twee) { return (Vec2i){een.x - twee.x, een.y - twee.y}; }
Vec3i Vec3iM(Vec3i een, Vec3i twee) { return (Vec3i){een.x - twee.x, een.y - twee.y, een.z - twee.z}; }
Vec4i Vec4iM(Vec4i een, Vec4i twee) { return (Vec4i){een.x - twee.x, een.y - twee.y, een.z - twee.z, een.w - twee.w}; }
Vec2u Vec2uM(Vec2u een, Vec2u twee) { return (Vec2u){een.x - twee.x, een.y - twee.y}; }
Vec3u Vec3uM(Vec3u een, Vec3u twee) { return (Vec3u){een.x - twee.x, een.y - twee.y, een.z - twee.z}; }
Vec4u Vec4uM(Vec4u een, Vec4u twee) { return (Vec4u){een.x - twee.x, een.y - twee.y, een.z - twee.z, een.w - twee.w}; }
Vec2f Vec2fM(Vec2f een, Vec2f twee) { return (Vec2f){een.x - twee.x, een.y - twee.y}; }
Vec3f Vec3fM(Vec3f een, Vec3f twee) { return (Vec3f){een.x - twee.x, een.y - twee.y, een.z - twee.z}; }
Vec4f Vec4fM(Vec4f een, Vec4f twee) { return (Vec4f){een.x - twee.x, een.y - twee.y, een.z - twee.z, een.w - twee.w}; }
Vec2d Vec2dM(Vec2d een, Vec2d twee) { return (Vec2d){een.x - twee.x, een.y - twee.y}; }
Vec3d Vec3dM(Vec3d een, Vec3d twee) { return (Vec3d){een.x - twee.x, een.y - twee.y, een.z - twee.z}; }
Vec4d Vec4dM(Vec4d een, Vec4d twee) { return (Vec4d){een.x - twee.x, een.y - twee.y, een.z - twee.z, een.w - twee.w}; }

/*	KEER	*/

Vec2i Vec2ii(Vec2i v, int a) { return (Vec2i){a * v.x, a * v.y}; }
Vec3i Vec3ii(Vec3i v, int a) { return (Vec3i){a * v.x, a * v.y, a * v.z}; }
Vec4i Vec4ii(Vec4i v, int a) { return (Vec4i){a * v.x, a * v.y, a * v.z, a * v.w}; }
Vec2u Vec2uu(Vec2u v, unsigned int a) { return (Vec2u){a * v.x, a * v.y}; }
Vec3u Vec3uu(Vec3u v, unsigned int a) { return (Vec3u){a * v.x, a * v.y, a * v.z}; }
Vec4u Vec4uu(Vec4u v, unsigned int a) { return (Vec4u){a * v.x, a * v.y, a * v.z, a * v.w}; }
Vec2f Vec2ff(Vec2f v, float a) { return (Vec2f){a * v.x, a * v.y}; }
Vec3f Vec3ff(Vec3f v, float a) { return (Vec3f){a * v.x, a * v.y, a * v.z}; }
Vec4f Vec4ff(Vec4f v, float a) { return (Vec4f){a * v.x, a * v.y, a * v.z, a * v.w}; }
Vec2d Vec2dd(Vec2d v, double a) { return (Vec2d){a * v.x, a * v.y}; }
Vec3d Vec3dd(Vec3d v, double a) { return (Vec3d){a * v.x, a * v.y, a * v.z}; }
Vec4d Vec4dd(Vec4d v, double a) { return (Vec4d){a * v.x, a * v.y, a * v.z, a * v.w}; }

/*	INPRODUCT	*/

int inproduct2i(Vec2i a, Vec2i b) { return a.x * b.x + a.y * b.y; }
int inproduct3i(Vec3i a, Vec3i b) { return a.x * b.x + a.y * b.y + a.z * b.y; }
int inproduct4i(Vec4i a, Vec4i b) { return a.x * b.x + a.y * b.y + a.z * b.y + a.w * b.w; }

float inproduct2f(Vec2f a, Vec2f b) { return a.x * b.x + a.y * b.y; }
float inproduct3f(Vec3f a, Vec3f b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
float inproduct4f(Vec4f a, Vec4f b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }

/*	UITPRODUCT	*/

Vec3i uitproducti(Vec3i a, Vec3i b) { return (Vec3i){a.y * b.z - b.y * a.z, a.z * b.x - b.z * a.x, a.x * b.y - b.x * a.y}; }
Vec3f uitproductf(Vec3f a, Vec3f b) { return (Vec3f){a.y * b.z - b.y * a.z, a.z * b.x - b.z * a.x, a.x * b.y - b.x * a.y}; }

/*	LENGTE	*/

float Vec3f_lengte_kw(Vec3f v) { return v.x * v.x + v.y * v.y + v.z * v.z; }
float Vec3f_lengte(Vec3f v) { return sqrtf(Vec3f_lengte_kw(v)); }

/*	NORMALISATIE	*/

Vec3f Vec3fn(Vec3f v) {
	float lengte = Vec3f_lengte(v);
	return Vec3ff(v, 1 / lengte);
}

/*	OVERGANG	*/

Vec4f Vec3n4f(Vec3f v, float w) { return (Vec4f){v.x, v.y, v.z, w}; }
Vec3f Vec4n3f(Vec4f v, booleaan verwerp_w) {
	if (verwerp_w) {
		return (Vec3f){v.x, v.y, v.z};
	} else {
		return (Vec3f){v.x / v.w, v.y / v.w, v.z / v.w};
	}
}

Vec3f Vec3dn3f(Vec3d v) { return (Vec3f){(float)v.x, (float)v.y, (float)v.z}; }

/**		MATRIXEN		**/

/*	KANTEL	*/

Mat4f kantelMat4f(Mat4f m) {
	return (Mat4f){{m.k1.x, m.k2.x, m.k3.x, m.k4.x},
				   {m.k1.y, m.k2.y, m.k3.y, m.k4.y},
				   {m.k1.z, m.k2.z, m.k3.z, m.k4.z},
				   {m.k1.w, m.k2.w, m.k3.w, m.k4.w}};
}

/*	PLUS	*/

/*	MIN	*/

/*	KEER	*/

Vec4f Mat4fVec4f(Mat4f L, Vec4f R) {
	return (Vec4f){L.k1.x * R.x + L.k2.x * R.y + L.k3.x * R.z + L.k4.x * R.w, L.k1.y * R.x + L.k2.y * R.y + L.k3.y * R.z + L.k4.y * R.w,
				   L.k1.z * R.x + L.k2.z * R.y + L.k3.z * R.z + L.k4.z * R.w, L.k1.w * R.x + L.k2.w * R.y + L.k3.w * R.z + L.k4.w * R.w};
}

Mat4f Mat4fMat4f(Mat4f L, Mat4f R) {
	return (Mat4f){{L.k1.x * R.k1.x + L.k2.x * R.k1.y + L.k3.x * R.k1.z + L.k4.x * R.k1.w,
					L.k1.y * R.k1.x + L.k2.y * R.k1.y + L.k3.y * R.k1.z + L.k4.y * R.k1.w,
					L.k1.z * R.k1.x + L.k2.z * R.k1.y + L.k3.z * R.k1.z + L.k4.z * R.k1.w,
					L.k1.w * R.k1.x + L.k2.w * R.k1.y + L.k3.w * R.k1.z + L.k4.w * R.k1.w},
				   {L.k1.x * R.k2.x + L.k2.x * R.k2.y + L.k3.x * R.k2.z + L.k4.x * R.k2.w,
					L.k1.y * R.k2.x + L.k2.y * R.k2.y + L.k3.y * R.k2.z + L.k4.y * R.k2.w,
					L.k1.z * R.k2.x + L.k2.z * R.k2.y + L.k3.z * R.k2.z + L.k4.z * R.k2.w,
					L.k1.w * R.k2.x + L.k2.w * R.k2.y + L.k3.w * R.k2.z + L.k4.w * R.k2.w},
				   {L.k1.x * R.k3.x + L.k2.x * R.k3.y + L.k3.x * R.k3.z + L.k4.x * R.k3.w,
					L.k1.y * R.k3.x + L.k2.y * R.k3.y + L.k3.y * R.k3.z + L.k4.y * R.k3.w,
					L.k1.z * R.k3.x + L.k2.z * R.k3.y + L.k3.z * R.k3.z + L.k4.z * R.k3.w,
					L.k1.w * R.k3.x + L.k2.w * R.k3.y + L.k3.w * R.k3.z + L.k4.w * R.k3.w},
				   {L.k1.x * R.k4.x + L.k2.x * R.k4.y + L.k3.x * R.k4.z + L.k4.x * R.k4.w,
					L.k1.y * R.k4.x + L.k2.y * R.k4.y + L.k3.y * R.k4.z + L.k4.y * R.k4.w,
					L.k1.z * R.k4.x + L.k2.z * R.k4.y + L.k3.z * R.k4.z + L.k4.z * R.k4.w,
					L.k1.w * R.k4.x + L.k2.w * R.k4.y + L.k3.w * R.k4.z + L.k4.w * R.k4.w}};
}

/*	DETERMINANT	*/

/*	ADJUDANT	*/

/*	INVERSE	*/

/*	DRAAI	*/

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

/*	HANDIG	*/

Mat4f verplaatsMatrix(float x, float y, float z) { return (Mat4f){{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {x, y, z, 1}}; }
Mat4f perspectiefMatrix(float voorvlak, float achtervlak, double zichthoek, float schermverhouding) {
	float invtan = 1 / tan(M_PI / 180.0 * zichthoek / 2.0);
	float invdelta = 1 / (achtervlak - voorvlak);
	return (Mat4f){{invtan, 0, 0, 0},
				   {0, schermverhouding * invtan, 0, 0},
				   {0, 0, (voorvlak + achtervlak) * invdelta, 1},
				   {0, 0, -2 * voorvlak * achtervlak * invdelta, 0}};
	// Mat Projec:
	// |1/tan(a/2)	0			0			0			|
	// |0			R/tan(a/2)	0			0			|
	// |0			0			(A+V)/(A-V)	-2AV/(A-V)	|
	// |0			0			1			0			|
}
Mat4f voorwerpMatrixPlekGrootte(Vec3f P, Vec3f G) { return (Mat4f){{G.x, 0, 0, 0}, {0, G.y, 0, 0}, {0, 0, G.z, 0}, {P.x, P.y, P.z, 1}}; }

/**		PRINTEN		**/

void printVec2f(Vec2f* v) { printf("[%f, %f]\n", v->x, v->y); }
void printVec3f(Vec3f* v) { printf("[%f, %f, %f]\n", v->x, v->y, v->z); }
void printVec4f(Vec4f* v) { printf("[%f, %f, %f, %f]\n", v->x, v->y, v->z, v->w); }
void printMat4f(Mat4f* m) {
	Mat4f gekanteld = kantelMat4f(*m);
	putchar('{');
	putchar('\n');
	printVec4f(&gekanteld.k1);
	printVec4f(&gekanteld.k2);
	printVec4f(&gekanteld.k3);
	printVec4f(&gekanteld.k4);
	putchar('}');
}
