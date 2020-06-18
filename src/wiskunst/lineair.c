#include "lineair.h"

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
