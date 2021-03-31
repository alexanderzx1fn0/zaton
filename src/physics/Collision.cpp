#include "Collision.h"

float Dot(const vec3& l, const vec3& r) {
	return l.x * r.x + l.y * r.y + l.z * r.z;
}

float Magnitude(const vec3& v) {
	return sqrtf(Dot(v, v));
}
vec3 Normalized(const vec3& v) {
	return v * (1.0f / Magnitude(v));
}

vec3 Cross(const vec3& l, const vec3& r) {
	vec3 result;
	result.x = l.y * r.z - l.z * r.y;
	result.y = l.z * r.x - l.x * r.z;
	result.z = l.x * r.y - l.y * r.x;
	return result;
}

Plane FromTriangle(const Triangle& t) {
	Plane result;
	result.normal = Normalized(Cross(t.b - t.a, t.c - t.a));
	result.distance = Dot(result.normal, t.a);
	return result;
}
