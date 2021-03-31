#include "utils.h"

bool SphereSphere(const Sphere& s1, const Sphere& s2) {
	float radiiSum = s1.radius + s2.radius;
	vec3 t = s1.center - s2.center;
	float sqDistance = t.length2();
	return sqDistance < radiiSum * radiiSum;
}

int32 clamp(int32 x, int32 a, int32 b) {
    if (x < a) return a;
    if (x > b) return b;
    return x;
}

float clamp(float x, float a, float b) {
    if (x < a) return a;
    if (x > b) return b;
    return x;
}

Triangle::Triangle(const vec3& a, const vec3& b, const vec3& c) : a(a), b(b), c(c) {}

vec3 Triangle::closestPoint(const vec3& p) const {
    vec3 ab = b - a;
    vec3 ac = c - a;
    vec3 ap = p - a;
    float d1 = ab.dot(ap);
    float d2 = ac.dot(ap);
    if (d1 <= 0.0f && d2 <= 0.0f) return a; // barycentric coordinates (1,0,0)

    // Check if P in vertex region outside B
    vec3 bp = p - b;
    float d3 = ab.dot(bp);
    float d4 = ac.dot(bp);
    if (d3 >= 0.0f && d4 <= d3) return b; // barycentric coordinates (0,1,0)

    float vc = d1 * d4 - d3 * d2;
    if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f) {
	float v = d1 / (d1 - d3);
	return a + ab * v; // barycentric coordinates (1-v,v,0)
    }

    vec3 cp = p - c;
    float d5 = ab.dot(cp);
    float d6 = ac.dot(cp);
    if (d6 >= 0.0f && d5 <= d6) return c; // barycentric coordinates (0,0,1)

    float vb = d5 * d2 - d1 * d6;
    if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f) {
	float w = d2 / (d2 - d6);
	return a + ac * w; // barycentric coordinates (1-w,0,w)
    }

    float va = d3 * d6 - d5 * d4;
    if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f) {
	float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
	return b + (c - b) * w; // barycentric coordinates (0,1-w,w)
    }

    float denom = 1.0f / (va + vb + vc);
    float v = vb * denom;
    float w = vc * denom;
    return a + ab * v + ac * w;
}

bool Triangle::intersect(const vec3& rayPos, const vec3& rayDir, bool twoSide, float& u, float& v, float& t) const {
    vec3 e1 = b - a;
    vec3 e2 = c - a;
    vec3 p = rayDir.cross(e2);
    float d = p.dot(e1);
    vec3 q;

    if (d > EPS) {
	vec3 r = rayPos - a;
	u = p.dot(r);
	if (u >= 0 && u <= d) {
	    q = r.cross(e1);
	    v = rayDir.dot(q);
	    if (v < 0 || u + v > d)
		return false;
	}
	else
	    return false;
    }
    else
	if (d < -EPS && twoSide) {
	    vec3 r = rayPos - a;
	    u = p.dot(r);
	    if (u <= 0 && u >= d) {
		q = r.cross(e1);
		v = rayDir.dot(q);
		if (v > 0 || u + v < d)
		    return false;
	    }
	    else
		return false;
	}
	else
	    return false;

    d = 1.0f / d;
    t = q.dot(e2);

    if (t > EPS) {
	u *= d;
	v *= d;
	t *= d;
	return true;
    }
    return false;
}
Sphere::Sphere(const vec3& center, float radius) : center(center), radius(radius) {}

bool Sphere::intersect(const Triangle& tri, vec3& n, float& t) {
    vec3 p = tri.closestPoint(center);
    vec3 v = center - p;

    float d = v.length2();
    if (d > radius * radius)
	return false;
    n = v.normal();
    t = radius - sqrtf(d);
    return true;
}
