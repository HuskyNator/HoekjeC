#include "lineair.h"

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

bool inproduct2b(Vec2b a, Vec2b b) {
	return a.x && b.x || a.y && b.y;
}
bool inproduct3b(Vec3b a, Vec3b b) {
	return a.x && b.x || a.y && b.y || a.z && b.z;
}
bool inproduct4b(Vec4b a, Vec4b b) {
	return a.x && b.x || a.y && b.y || a.z && b.z || a.w && b.w;
}

int inproduct2i(Vec2i a, Vec2i b) {
	return a.x * b.x + a.y * b.y;
}
int inproduct3i(Vec3i a, Vec3i b) {
	return a.x * b.x + a.y * b.y + a.z * b.y;
}
int inproduct4i(Vec4i a, Vec4i b) {
	return a.x * b.x + a.y * b.y + a.z * b.y + a.w * b.w;
}

float inproduct2f(Vec2f a, Vec2f b) {
	return a.x * b.x + a.y * b.y;
}
float inproduct3f(Vec3f a, Vec3f b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}
float inproduct4f(Vec4f a, Vec4f b) {
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

Vec3b uitproductb(Vec3b a, Vec3b b) {
	return (Vec3b){a.y && b.z || !(b.y && a.z), a.z && b.x || !(b.z * a.x), a.x && b.y || !(b.x * a.y)};
}

Vec3i uitproducti(Vec3i a, Vec3i b) {
	return (Vec3i){a.y * b.z - b.y * a.z, a.z * b.x - b.z * a.x, a.x * b.y - b.x * a.y};
}

Vec3f uitproductf(Vec3f a, Vec3f b) {
	return (Vec3f){a.y * b.z - b.y * a.z, a.z * b.x - b.z * a.x, a.x * b.y - b.x * a.y};
}

Mat4f m = (Mat4f){{1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}};