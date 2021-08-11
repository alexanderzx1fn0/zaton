#ifndef SKY_PLANE_H_
#define SKY_PLANE_H_

#include <vector>
#include <glm/vec3.hpp>

class SkyPlane
{
public:
    SkyPlane(); 
    ~SkyPlane(); 

    bool create();
    void remove();

    bool createBuffers();

    void draw();

    enum
    {
        DIV_X = 4,
        DIV_Y = 4,
        NUM_VERTICES = (DIV_X+1) * (DIV_Y+1),
        NUM_INDICES = 2*DIV_Y * (DIV_X+1) + (DIV_Y-1)*2,
        NUM_TRIANGLES = NUM_INDICES-2,
    };

    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;

    unsigned int _vao;
    unsigned int _vb;
    unsigned int _ib;
};

#endif
