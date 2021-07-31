#ifndef RANDOM_H_
#define RANDOM_H_

namespace math {

inline int RandomNumber(int min, int max)
{
    if (min == max) return (min);
    return ((rand()% (abs(max-min)+1)) +min);
}
inline float RandomNumber(float min, float max)
{
    if (min == max)
    {
        return min;
    }
    float random = (float)rand() / (float) RAND_MAX;
    return ((random * (float)fabs(max-min))+min);
}

inline float GetRandomFloat(float a, float b)
{
    if (a >= b)
        return a;
    float f = (rand()%10001) * 0.0001f;

    return (f*(b-a))+a;
}

inline void GetRandomVec(vec3& out)
{
    out.x = GetRandomFloat(-1.0f, 1.0f);
    out.y = GetRandomFloat(-1.0f, 1.0f);
    out.z = GetRandomFloat(-1.0f, 1.0f);

    out.normal();
}

}
#endif
