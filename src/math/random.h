#ifndef RANDOM_H_
#define RANDOM_H_

namespace math {

inline int RandomNumber(int _min, int _max)
{
    if (_min == _max) return (_min);
    return ((rand()% (abs(_max-_min)+1)) +_min);
}
inline float RandomNumber(float _min, float _max)
{
    if (_min == _max)
    {
        return _min;
    }
    float random = (float)rand() / (float) RAND_MAX;
    return ((random * (float)fabs(_max-_min))+_min);
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
