#ifndef PLANE_H_
#define PLANE_H_

#include "util/utils.h"

typedef vec3 Point;


typedef struct Plane {
	vec3 normal;
	float distance;

	inline Plane() : normal(1, 0, 0) { }
	inline Plane(const vec3& n, float d) :
		normal(n), distance(d) { }
} Plane;


float Dot(const vec3& l, const vec3& r);
float Magnitude(const vec2& v);
Plane FromTriangle(const Triangle& t);
vec3 Normalized(const vec3& v);
vec3 Cross(const vec3& l, const vec3& r);


#endif
