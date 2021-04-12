#include "Collision.h"
#include "game/Entity.h"

AABB::AABB()
    : transform()
{
    program = new Shader("../data/shaders/aabb_vertex.glsl",
			"../data/shaders/aabb_fragment.glsl");
    min = vec3(0.0f);
    max = vec3(0.0f);
    size = vec3(0.0f);
    center = vec3(0.0f);
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

float AABB::intersect(const vec3& ro, const vec3& rd) 
{ 

    float t1 = (min.x - ro.x) / rd.x;
    float t2 = (max.x - ro.x) /rd.x;
    float t3 = (min.y - ro.y) / rd.y;
    float t4 = (max.y - ro.y) / rd.y;
    float t5 = (min.z - ro.z) / rd.z;
    float t6 = (max.z - ro.z) / rd.z;

    float tmin = fmaxf(fmaxf(fminf(t1, t2),
                             fminf(t3, t4)
                             ),
                             fminf(t5, t6)
                             );

    float tmax = fminf(
                    fminf(
                        fmaxf(t1, t2),
                        fmaxf(t3, t4)
                    ),
                    fmaxf(t5, t6)
                    );
    if (tmax < 0) {
        return -1;
    }

    if (tmin > tmax)
    {
        return -1;
    }

    if (tmin < 0.0f) {
        return tmax;
    }

    return tmin;
}

void AABB::recompute()
{
    min.x += transform.e03;
    min.y += transform.e13;
    min.z += transform.e23;

    max.x += transform.e03;
    max.y += transform.e13;
    max.z += transform.e23;
/*
    min.x += transform.e03;
    min.y += transform.e13;
    min.z += transform.e23;

    max.x += transform.e03;
    max.y += transform.e13;
    max.z += transform.e23;
*/
}


void AABB::setUniform(const mat4* matViewProj)
{
    program->bind();
    glUniformMatrix4fv(program->getUniform("uModelM"), 1, GL_FALSE, (float*)&transform);
    glUniformMatrix4fv(program->getUniform("uViewProjM"), 1, GL_FALSE, (float*)matViewProj);
}

void AABB::generateBox()
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
  // Cube 1x1x1, centered on origin
GLfloat vertices[] = {
-0.5, -0.5, -0.5, 1.0,
 0.5, -0.5, -0.5, 1.0,
 0.5,  0.5, -0.5, 1.0,
-0.5,  0.5, -0.5, 1.0,
-0.5, -0.5,  0.5, 1.0,
 0.5, -0.5,  0.5, 1.0,
 0.5,  0.5,  0.5, 1.0,
-0.5,  0.5,  0.5, 1.0,
};
glGenBuffers(1, &vbo);
glBindBuffer(GL_ARRAY_BUFFER, vbo);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(
    0,  // attribute
    4,                  // number of elements per vertex, here (x,y,z,w)
    GL_FLOAT,           // the type of each element
    GL_FALSE,           // take our values as-is
    0,                  // no extra data between each position
    0                   // offset of first element
  );


GLushort elements[] = {
0, 1, 2, 3,
4, 5, 6, 7,
0, 4, 1, 5, 2, 6, 3, 7
};
glGenBuffers(1, &ebo);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);


    glBindVertexArray(0);
}

void AABB::draw()
{
/*
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
*/
    glBindVertexArray(vao);
    glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, 0);
    glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, (GLvoid*)(4*sizeof(GLushort)));
    glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, (GLvoid*)(8*sizeof(GLushort)));
    glBindVertexArray(0);
}

void AABB::computeAABB(Entity* entity)
{
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
                    max.x = entity->obj.f_vertices[entity->obj.indices[i]].p.x;
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

        size = vec3(max.x - min.x, max.y - min.y, max.z - min.z);
        center = vec3( (max.x + min.x) * .5, (max.y + min.y) * .5, (max.z + min.z) * .5);

        transform.identity();
        transform.translate(center);
        transform.scale(size);
        transform = entity->obj.matrix * transform; // apply transformation
/*
        min.x += entity->obj.matrix.e03;
        min.y += entity->obj.matrix.e13;
        min.z += entity->obj.matrix.e23;

        max.x += entity->obj.matrix.e03;
        max.y += entity->obj.matrix.e13;
        max.z += entity->obj.matrix.e23;
*/
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
