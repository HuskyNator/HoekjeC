#ifndef LINEAR_H
#define LINEAR_H

typedef enum { false, true } bool;

// Vectoren:
typedef struct vec2b Vec2b;
typedef struct vec3b Vec3b;
typedef struct vec4b Vec4b;

typedef struct vec2i Vec2i;
typedef struct vec3i Vec3i;
typedef struct vec4i Vec4i;

typedef struct vec2f Vec2f;
typedef struct vec3f Vec3f;
typedef struct vec4f Vec4f;

// Matrices:
typedef struct mat2b Mat2b;
typedef struct mat3b Mat3b;
typedef struct mat4b Mat4b;

typedef struct mat2i Mat2i;
typedef struct mat3i Mat3i;
typedef struct mat4i Mat4i;

typedef struct mat2f Mat2f;
typedef struct mat3f Mat3f;
typedef struct mat4f Mat4f;

// Inproducten:
bool inproduct2b(Vec2b a, Vec2b b);
bool inproduct3b(Vec3b a, Vec3b b);
bool inproduct4b(Vec4b a, Vec4b b);

int inproduct2i(Vec2i a, Vec2i b);
int inproduct3i(Vec3i a, Vec3i b);
int inproduct4i(Vec4i a, Vec4i b);

float inproduct2f(Vec2f a, Vec2f b);
float inproduct3f(Vec3f a, Vec3f b);
float inproduct4f(Vec4f a, Vec4f b);

// Uitproducten:
Vec3b uitproductb(Vec3b a, Vec3b b);
Vec3i uitproducti(Vec3i a, Vec3i b);
Vec3f uitproductf(Vec3f a, Vec3f b);

#endif