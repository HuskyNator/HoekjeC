#ifndef LINEAR_H
#define LINEAR_H

#include "HC/booleaan.h"

// Vectoren

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

typedef struct vec2ui Vec2ui;
typedef struct vec3ui Vec3ui;
typedef struct vec4ui Vec4ui;

struct vec2ui {
	unsigned int x;
	unsigned int y;
};
struct vec3ui {
	unsigned int x;
	unsigned int y;
	unsigned int z;
};
struct vec4ui {
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

// Matrixen.

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

// Inproducten

int inproduct2i(Vec2i a, Vec2i b);
int inproduct3i(Vec3i a, Vec3i b);
int inproduct4i(Vec4i a, Vec4i b);

float inproduct2f(Vec2f a, Vec2f b);
float inproduct3f(Vec3f a, Vec3f b);
float inproduct4f(Vec4f a, Vec4f b);

// Uitproducten

Vec3i uitproducti(Vec3i a, Vec3i b);
Vec3f uitproductf(Vec3f a, Vec3f b);

// Lente

float Vec3f_lengte_kw(Vec3f v);

// Overgangen

Vec3f Vec3fn(Vec3f v);
Vec4f Vec3n4f(Vec3f v, float w);
Vec3f Vec4n3f(Vec4f v, booleaan verwerp_w);

Vec3f Vec3dn3f(Vec3d v);

Mat4f kantelMat4f(Mat4f m);

// Vermenigvuldigingen

Vec4f Vec4ff(Vec4f v, float f);
Vec3f Vec3ff(Vec3f v, float f);
Mat4f Mat4fMat4f(Mat4f L, Mat4f R);
Vec4f Mat4fVec4f(Mat4f L, Vec4f R);

// Draaiingen

Mat4f draaiMatrixx(float draai);
Mat4f draaiMatrixy(float draai);
Mat4f draaiMatrixz(float draai);

// Matrixen

Mat4f identiteitsMatrix();
Mat4f verplaatsMatrix(float x, float y, float z);
Mat4f perspectiefMatrix(float voorvlak, float achtervlak, double zichthoek, float schermverhouding);
Mat4f voorwerpMatrixPlekGrootte(Vec3f plek, Vec3f grootte);

// Printen
void printVec2f(Vec2f* v);
void printVec3f(Vec3f* v);
void printVec4f(Vec4f* v);
void printMat4f(Mat4f* m);

#endif