#include <stdio.h>
#include <stdlib.h>

class Sphere
{
public:
    Sphere(vec3 p, float r);

private:
    vec3 position;
    float radius;
};

int main()
{
    return EXIT_SUCCESS;
}
