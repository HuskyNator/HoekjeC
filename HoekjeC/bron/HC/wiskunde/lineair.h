#ifndef LINEAR_H
#define LINEAR_H

#include "HC/algemeen.h"

/**
 * @file
 * @short Verzameling van lineaire wiskunde.
 */

/**		VECTOREN		**/

typedef struct vec2i Vec2i;
typedef struct vec3i Vec3i;
typedef struct vec4i Vec4i;

struct vec2i {
	int x;
	int y;
};
struct vec3i {
	int x;
	int y;
	int z;
};
struct vec4i {
	int x;
	int y;
	int z;
	int w;
};

typedef struct vec2u Vec2u;
typedef struct vec3u Vec3u;
typedef struct vec4u Vec4u;

struct vec2u {
	unsigned int x;
	unsigned int y;
};
struct vec3u {
	unsigned int x;
	unsigned int y;
	unsigned int z;
};
struct vec4u {
	unsigned int x;
	unsigned int y;
	unsigned int z;
	unsigned int w;
};

typedef struct vec2f Vec2f;
typedef struct vec3f Vec3f;
typedef struct vec4f Vec4f;

struct vec2f {
	float x;
	float y;
};
struct vec3f {
	float x;
	float y;
	float z;
};
struct vec4f {
	float x;
	float y;
	float z;
	float w;
};

typedef struct vec2d Vec2d;
typedef struct vec3d Vec3d;
typedef struct vec4d Vec4d;

struct vec2d {
	double x;
	double y;
};
struct vec3d {
	double x;
	double y;
	double z;
};
struct vec4d {
	double x;
	double y;
	double z;
	double w;
};

/*	PLUS	*/

Vec2i Vec2iP(Vec2i een, Vec2i twee);
Vec3i Vec3iP(Vec3i een, Vec3i twee);
Vec4i Vec4iP(Vec4i een, Vec4i twee);

Vec2u Vec2uP(Vec2u een, Vec2u twee);
Vec3u Vec3uP(Vec3u een, Vec3u twee);
Vec4u Vec4uP(Vec4u een, Vec4u twee);

Vec2f Vec2fP(Vec2f een, Vec2f twee);
Vec3f Vec3fP(Vec3f een, Vec3f twee);
Vec4f Vec4fP(Vec4f een, Vec4f twee);

Vec2d Vec2dP(Vec2d een, Vec2d twee);
Vec3d Vec3dP(Vec3d een, Vec3d twee);
Vec4d Vec4dP(Vec4d een, Vec4d twee);

/*	MIN	*/

Vec2i Vec2iP(Vec2i een, Vec2i twee);
Vec3i Vec3iP(Vec3i een, Vec3i twee);
Vec4i Vec4iP(Vec4i een, Vec4i twee);

Vec2u Vec2uP(Vec2u een, Vec2u twee);
Vec3u Vec3uP(Vec3u een, Vec3u twee);
Vec4u Vec4uP(Vec4u een, Vec4u twee);

Vec2f Vec2fP(Vec2f een, Vec2f twee);
Vec3f Vec3fP(Vec3f een, Vec3f twee);
Vec4f Vec4fP(Vec4f een, Vec4f twee);

Vec2d Vec2dP(Vec2d een, Vec2d twee);
Vec3d Vec3dP(Vec3d een, Vec3d twee);
Vec4d Vec4dP(Vec4d een, Vec4d twee);

/*	KEER	*/

Vec2i Vec2ii(Vec2i v, int a);
Vec3i Vec3ii(Vec3i v, int a);
Vec4i Vec4ii(Vec4i v, int a);
Vec2u Vec2uu(Vec2u v, unsigned int a);
Vec3u Vec3uu(Vec3u v, unsigned int a);
Vec4u Vec4uu(Vec4u v, unsigned int a);
Vec2f Vec2ff(Vec2f v, float a);
Vec3f Vec3ff(Vec3f v, float a);
Vec4f Vec4ff(Vec4f v, float a);
Vec2d Vec2dd(Vec2d v, double a);
Vec3d Vec3dd(Vec3d v, double a);
Vec4d Vec4dd(Vec4d v, double a);

/*	INPRODUCT	*/

int inproduct2i(Vec2i a, Vec2i b);
int inproduct3i(Vec3i a, Vec3i b);
int inproduct4i(Vec4i a, Vec4i b);

float inproduct2f(Vec2f a, Vec2f b);
float inproduct3f(Vec3f a, Vec3f b);
float inproduct4f(Vec4f a, Vec4f b);

/*	UITPRODUCT	*/

Vec3i uitproducti(Vec3i a, Vec3i b);
Vec3f uitproductf(Vec3f a, Vec3f b);

/*	LENGTE	*/

float Vec3f_lengte_kw(Vec3f v);
float Vec3f_lengte(Vec3f v);

/*	NORMALISATIE	*/

Vec3f Vec3fn(Vec3f v);

/*	OVERGANG	*/

Vec4f Vec3n4f(Vec3f v, float w);
Vec3f Vec4n3f(Vec4f v, booleaan verwerp_w);

Vec3f Vec3dn3f(Vec3d v);

/**		MATRIXEN		**/

typedef struct mat2i Mat2i;
typedef struct mat3i Mat3i;
typedef struct mat4i Mat4i;

struct mat2i {
	Vec2i r1;
	Vec2i r2;
};
struct mat3i {
	Vec3i r1;
	Vec3i r2;
	Vec3i r3;
};
struct mat4i {
	Vec4i r1;
	Vec4i r2;
	Vec4i r3;
	Vec4i r4;
};

typedef struct mat2f Mat2f;
typedef struct mat3f Mat3f;
typedef struct mat4f Mat4f;

struct mat2f {
	Vec2f k1;
	Vec2f k2;
};
struct mat3f {
	Vec3f k1;
	Vec3f k2;
	Vec3f k3;
};
struct mat4f {
	Vec4f k1;
	Vec4f k2;
	Vec4f k3;
	Vec4f k4;
};

/*	KANTEL	*/

Mat4f kantelMat4f(Mat4f m);

/*	PLUS	*/

/*	MIN	*/

/*	KEER	*/

Vec4f Mat4fVec4f(Mat4f L, Vec4f R);
Mat4f Mat4fMat4f(Mat4f L, Mat4f R);

/*	DETERMINANT	*/

/*	ADJUDANT	*/

/*	INVERSE	*/

/*	DRAAI	*/

Mat4f draaiMatrixx(float draai);
Mat4f draaiMatrixy(float draai);
Mat4f draaiMatrixz(float draai);

/*	HANDIG	*/

#define identiteitsMatrix                                        \
	(Mat4f) {                                                    \
		{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, { 0, 0, 0, 1 } \
	}
Mat4f verplaatsMatrix(float x, float y, float z);
Mat4f perspectiefMatrix(float voorvlak, float achtervlak, double zichthoek, float schermverhouding);
Mat4f voorwerpMatrixPlekGrootte(Vec3f plek, Vec3f grootte);

/**		PRINTEN		**/

void printVec2f(Vec2f* v);
void printVec3f(Vec3f* v);
void printVec4f(Vec4f* v);
void printMat4f(Mat4f* m);

#endif