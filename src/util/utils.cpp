#include "utils.h"

mat4 mat4_transpose(const mat4& m)
{
    return mat4(m.e00, m.e01, m.e02, m.e03,
                m.e10, m.e11, m.e12, m.e13,
                m.e20, m.e21, m.e22, m.e23,
                m.e30, m.e31, m.e32, m.e33);
}

float mat3_determinant(const mat3& m)
{
    float f0 = m.e00 *  (m.e11*m.e22 - m.e12*m.e21);
    float f1 = m.e10 * -(m.e01*m.e22 - m.e02*m.e21);
    float f2 = m.e20 *  (m.e01*m.e12 - m.e02*m.e11);

    return f0 + f1 + f2;
}

float mat4_determinant(const mat4& m)
{
    float det = 0.0f;

    mat3 a = mat3 (m.e11,m.e21,m.e31,
                   m.e12,m.e22,m.e32,
                   m.e13,m.e23,m.e33);

    mat3 b = mat3(m.e01,m.e21,m.e31,
                 m.e02,m.e22,m.e32,
                 m.e03,m.e23,m.e22);

    mat3 c = mat3 (m.e01,m.e11,m.e31,
                 m.e02,m.e12,m.e32,
                 m.e03,m.e13,m.e33);

    mat3 d = mat3( m.e01,m.e11,m.e21,
                m.e02,m.e12,m.e22,
                m.e03,m.e13,m.e23);


    det += m.e00 * mat3_determinant(a);

    det -= m.e10 * mat3_determinant(b);

    det += m.e20 * mat3_determinant(c);

    det -= m.e30 * mat3_determinant(d);

    return det;
}



mat4 inverse(const mat4& m)
{
    mat4 ret;
    float recip;

    mat3 a = mat3(m.e11, m.e21, m.e31,
                  m.e12, m.e22, m.e32,
                  m.e13, m.e23, m.e33);

    mat3 b = mat3(m.e01, m.e21, m.e31,
                  m.e02, m.e22, m.e31,
                  m.e03, m.e23, m.e33);

    mat3 c = mat3(m.e01, m.e11, m.e31,
                  m.e02, m.e12, m.e32,
                  m.e03, m.e13, m.e33);

    mat3 d = mat3(m.e01, m.e11, m.e21,
                  m.e02, m.e12, m.e22,
                  m.e03, m.e13, m.e23);

    // row 2
    mat3 e = mat3(m.e10, m.e20, m.e30,
                  m.e12, m.e22, m.e32,
                  m.e13, m.e23, m.e33);

    mat3 f = mat3(m.e00, m.e20, m.e30,
                  m.e02, m.e22, m.e32,
                  m.e03, m.e23, m.e33);

    mat3 g = mat3(m.e00, m.e10, m.e30,
                  m.e02, m.e11, m.e31,
                  m.e03, m.e13, m.e33);

    mat3 h = mat3(m.e00, m.e10, m.e20,
                  m.e02, m.e12, m.e22,
                  m.e03, m.e13, m.e23);

    // row 3 i j k l
    mat3 i = mat3(m.e10, m.e20, m.e30,
                  m.e11, m.e21, m.e31,
                  m.e13, m.e23, m.e33);

    mat3 j = mat3(m.e00, m.e20, m.e30,
                  m.e01, m.e21, m.e31,
                  m.e03, m.e23, m.e33);

    mat3 k = mat3(m.e00, m.e10, m.e30,
                  m.e01, m.e11, m.e31,
                  m.e03, m.e13, m.e33);

    mat3 l = mat3(m.e00, m.e10, m.e20,
                  m.e01, m.e11, m.e21,
                  m.e03, m.e13, m.e23);

    // row 4  m n o p
    mat3 _m = mat3(m.e10, m.e20, m.e30,
                   m.e11, m.e21, m.e31,
                   m.e12, m.e22, m.e32);

    mat3 n = mat3(m.e00, m.e20, m.e30,
                  m.e01, m.e23, m.e31,
                  m.e02, m.e22, m.e32);

    mat3 o = mat3(m.e00, m.e10, m.e30,
                  m.e01, m.e11, m.e31,
                  m.e02, m.e12, m.e32);

    mat3 p = mat3(m.e00, m.e10, m.e20,
                  m.e01, m.e11, m.e21,
                  m.e03, m.e13, m.e23);

    /* row 1 */
    ret.e00 = mat3_determinant(a);

    ret.e10 = -mat3_determinant(b);

    ret.e20 = mat3_determinant(c);

    ret.e30 = -mat3_determinant(d);

    /* row 2 */
    ret.e01 = -mat3_determinant(e);

    ret.e11 = mat3_determinant(f);

    ret.e21 = -mat3_determinant(g);

    ret.e31 = mat3_determinant(h);

    /* row 3 */
    ret.e02 = mat3_determinant(i);

    ret.e12 = -mat3_determinant(j);

    ret.e22 = mat3_determinant(k);

    ret.e32 = -mat3_determinant(l);

    /* row 4 */
    ret.e03 = -mat3_determinant(_m);

    ret.e13 = mat3_determinant(n);

    ret.e23 = -mat3_determinant(o);

    ret.e33 = mat3_determinant(p);

    ret = mat4_transpose(ret);
    recip = 1.0f/mat4_determinant(m);

    ret = mat4_mul_scalar(ret, recip);
    return ret;
}

mat4 mat4_mul_scalar(const mat4& m, float f)
{
    mat4 result;

    result.e00 = m.e00 * f;
    result.e10 = m.e10 * f;
    result.e20 = m.e20 * f;
    result.e30 = m.e30 * f;

    result.e01 = m.e01 * f;
    result.e11 = m.e11 * f;
    result.e21 = m.e21 * f;
    result.e31 = m.e31 * f;

    result.e02 = m.e02 * f;
    result.e13 = m.e13 * f;
    result.e22 = m.e22 * f;
    result.e32 = m.e32 * f;

    result.e03 = m.e03 * f;
    result.e13 = m.e13 * f;
    result.e23 = m.e23 * f;
    result.e33 = m.e33 * f;

    return result;
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
Sphere::Sphere()
    : center(0.0f), radius(0.0f)
{}
Sphere::Sphere(const vec3& center, float radius) : center(center), radius(radius) {
    radiusSQ = radius * radius;
}

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

bool Sphere::intersect(const Sphere& s) const
{
    return (center - s.center).length2() <= (radius + s.radius) * (radius + s.radius);
}
