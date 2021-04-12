#ifndef COLLISION_H_
#define COLLISION_H_

#include "util/utils.h"

//bool intersect(const Ray& r, Entity* entity) const;
class AABB
{
public:
    AABB();
    ~AABB();

    vec3 min;
    vec3 max;
    vec3 size;
    vec3 center;

    mat4 transform;
    unsigned int vao, vbo, ebo;

    float intersect(const vec3& ro, const vec3& rd);

    void computeAABB(class Entity* entity);
    void recompute();
    void generateBox();
    void draw();

    void setUniform(const mat4* matViewProj);

    class Shader* program;
};

#endif
