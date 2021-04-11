#include "Collision.h"
#include "game/Entity.h"

AABB::AABB()
    : transform()
{
    program = new Shader("../data/shaders/aabb_vertex.glsl",
			"../data/shaders/aabb_fragment.glsl");
}

AABB::~AABB()
{
    delete program;

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &ebo);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vao);
}


void AABB::setUniform(const mat4* matViewProj)
{
    program->bind();
    glUniformMatrix4fv(program->getUniform("uModelM"), 1, GL_FALSE, (float*)&transform);
    glUniformMatrix4fv(program->getUniform("uViewProjM"), 1, GL_FALSE, (float*)matViewProj);
}

void AABB::generateBox()
{
    static const vec3 kCubeVertices[] =
    {
        vec3(  1.0f,  1.0f, -1.0f ), /* 0 */
        vec3( -1.0f,  1.0f, -1.0f ), /* 1 */
        vec3( -1.0f, -1.0f, -1.0f ), /* 2 */
        vec3( 1.0f, -1.0f, -1.0f ), /* 3 */
        vec3(  1.0f,  1.0f,  1.0f ), /* 4 */
        vec3( -1.0f,  1.0f,  1.0f ), /* 5 */
        vec3( -1.0f, -1.0f,  1.0f ), /* 6 */
        vec3(  1.0f, -1.0f,  1.0f ), /* 7 */
    };

    static const unsigned int kCubeIndices[] =
    {
        0, 2, 1,   0, 3, 2,  /* front */
        4, 3, 0,   4, 7, 3,  /* right */
        4, 1, 5,   4, 0, 1,  /* top */
        1, 6, 5,   1, 2, 6,  /* left */
        3, 6, 2,   3, 7, 6,  /* bottom */
        5, 7, 4,   5, 6, 7,  /* back */
    };

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float) * 3, kCubeVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // position
    glEnableVertexAttribArray( 0 );

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(unsigned int), kCubeIndices, GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void AABB::draw()
{
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void AABB::computeAABB(Entity* entity)
{
        vec3 min = vec3(0.0f);
        vec3 max = vec3(0.0f);

        min.x = max.x = entity->obj.f_vertices[entity->obj.indices[0]].p.x;
        min.y = max.y = entity->obj.f_vertices[entity->obj.indices[1]].p.y;
        min.z = max.z = entity->obj.f_vertices[entity->obj.indices[2]].p.z;

        for (int i = 0; i < entity->obj.nIndices; i += 3) {
            if (entity->obj.f_vertices[entity->obj.indices[i]].p.x < min.x) 
            {
                    min.x = entity->obj.f_vertices[entity->obj.indices[i]].p.x;
            }

            if (entity->obj.f_vertices[entity->obj.indices[i]].p.x > max.x) 
            {
                    min.x = entity->obj.f_vertices[entity->obj.indices[i]].p.x;
            }

            if (entity->obj.f_vertices[entity->obj.indices[i]].p.y < min.y) 
            {
                    min.y = entity->obj.f_vertices[entity->obj.indices[i]].p.y;
            }

            if (entity->obj.f_vertices[entity->obj.indices[i]].p.y > max.y) 
            {
                    max.y = entity->obj.f_vertices[entity->obj.indices[i]].p.y;
            }

            if (entity->obj.f_vertices[entity->obj.indices[i]].p.z < min.z) 
            {
                    min.z = entity->obj.f_vertices[entity->obj.indices[i]].p.z;
            }
            if (entity->obj.f_vertices[entity->obj.indices[i]].p.z > max.z) 
            {
                    max.z = entity->obj.f_vertices[entity->obj.indices[i]].p.z;
            }		
        }

        vec3 size = vec3(max.x - min.x, max.y - min.y, max.z - min.z);
        vec3 center = vec3( (max.x + min.x) * .5, (max.y + min.y) * .5, (max.z + min.z) * .5);

        transform.identity();
        transform = transform * entity->obj.matrix;
        transform.scale(size);
        transform.translate(center);
/*
            printf("MIN: %f %f %f\n", min.x, min.y, min.z);
            printf("MAX: %f %f %f\n", max.x, max.y, max.z);
            printf("Next entity\n");
*/
            //printf("%f %f %f\n", center.x, center.y, center.z);
}

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
