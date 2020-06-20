#ifndef LINEAR_H
#define LINEAR_H

typedef enum { false, true } bool;

// Vectoren.
typedef struct vec2b Vec2b;
typedef struct vec3b Vec3b;
typedef struct vec4b Vec4b;

struct vec2b {
	bool x;
	bool y;
};
struct vec3b {
	bool x;
	bool y;
	bool z;
};
struct vec4b {
	bool x;
	bool y;
	bool z;
	bool w;
};

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

// Matrixen.
typedef struct mat2b Mat2b;
typedef struct mat3b Mat3b;
typedef struct mat4b Mat4b;

struct mat2b {
	Vec2b r1;
	Vec2b r2;
};
struct mat3b {
	Vec3b r1;
	Vec3b r2;
	Vec3b r3;
};
struct mat4b {
	Vec4b r1;
	Vec4b r2;
	Vec4b r3;
	Vec4b r4;
};

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
	Vec2f r1;
	Vec2f r2;
};
struct mat3f {
	Vec3f r1;
	Vec3f r2;
	Vec3f r3;
};
struct mat4f {
	Vec4f r1;
	Vec4f r2;
	Vec4f r3;
	Vec4f r4;
};

// Inproducten.
bool inproduct2b(Vec2b a, Vec2b b);
bool inproduct3b(Vec3b a, Vec3b b);
bool inproduct4b(Vec4b a, Vec4b b);

int inproduct2i(Vec2i a, Vec2i b);
int inproduct3i(Vec3i a, Vec3i b);
int inproduct4i(Vec4i a, Vec4i b);

float inproduct2f(Vec2f a, Vec2f b);
float inproduct3f(Vec3f a, Vec3f b);
float inproduct4f(Vec4f a, Vec4f b);

// Uitproducten.
Vec3b uitproductb(Vec3b a, Vec3b b);
Vec3i uitproducti(Vec3i a, Vec3i b);
Vec3f uitproductf(Vec3f a, Vec3f b);

// Matrix Toepassingen.
Mat4f identiteitsMatrix();

Mat4f vermenigvuldigMatrix(Mat4f L, Mat4f R);

Mat4f verplaatsMatrix(float x, float y, float z);

Mat4f draaiMatrixx(float draai);
Mat4f draaiMatrixy(float draai);
Mat4f draaiMatrixz(float draai);

Mat4f perspectiefMatrix(float voorvlak, float achtervlak, double zichthoek, float schermverhouding);
Mat4f voorwerpMatrixPG(Vec3f P, Vec3f G);

#endif