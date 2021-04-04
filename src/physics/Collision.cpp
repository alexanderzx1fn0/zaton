#include "Collision.h"

// each Entity have bounding volume, for example Sphere
// class 
/*
bool intersect(const Ray& r, Entity* entity) const
{
    float t0, t1;

    vec3 L = s.center - r.origin;
    float tca = l.dot(r.direction);
    float d2 = l.dot(l) - tca*tca;
    if (d > s.radiusSQ)
    {
        return false;
    }

    float thc = sqrt(s.radiusSQ - d2);

    t0 = tca - thc;
    t1 = tca + thc;

    if (t0 > t1) { swap(t0, t1); }
    if (t0 < 0.0f) {
        t0 = t1;
        if (t0 < 0) {return false; }
    }

    return true;
}
*/
