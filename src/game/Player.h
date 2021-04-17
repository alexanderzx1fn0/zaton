#ifndef PLAYER_H_
#define PLAYER_H_

#include "BasePlayer.h"
#include "util/utils.h"


#define PLAYER_RADIUS           0.25f
#define PLAYER_RADIUS_GROUND    0.15f
#define PLAYER_HEIGHT           1.75f
#define PLAYER_MAX_SPEED        8.0f
#define PLAYER_ACCEL_GROUND     10.0f
#define PLAYER_ACCEL_AIR        5.0f
#define PLAYER_FRICTION_GROUND  0.999f
#define PLAYER_FRICTION_AIR     0.7f
#define PLAYER_FRICTION_STOP    0.025f
#define PLAYER_JUMP_SPEED       8.0f
#define GRAVITY  2.0f * 9.81f // 2.0f * 9.81f


constexpr float PITCH_OFFSET_R    = 0.0f;   // Насколько сильно ствол смещается вбок (влево) при вертикальных поворотах камеры
constexpr float PITCH_OFFSET_N    = 0.0f;   // Насколько сильно ствол поднимается\опускается при вертикальных поворотах камеры
constexpr float PITCH_OFFSET_D    = 0.02f;  // Насколько сильно ствол приближается\отдаляется при вертикальных поворотах камеры
constexpr float PITCH_LOW_LIMIT   = -PI;    // Минимальное значение pitch при использовании совместно с PITCH_OFFSET_N
constexpr float ORIGIN_OFFSET     = -0.05f; // Фактор влияния инерции на положение ствола (чем меньше, тем масштабней инерция)
constexpr float ORIGIN_OFFSET_AIM = -0.03f; // (Для прицеливания)
constexpr float TENDTO_SPEED      = 5.f;    // Скорость нормализации положения ствола
constexpr float TENDTO_SPEED_AIM  = 8.f;    // (Для прицеливания)



class Player : public BasePlayer {
public:
    enum PlayerType {
        PLAYER_1,
        PLAYER_2,
        PLAYER_NET,
    } type;

    enum {
        LEFT    = 1,
        RIGHT   = 2,
        UP      = 4,
        DOWN    = 8,
        JUMP    = 16,
        FIRE_A  = 32,
        FIRE_B  = 64,
    };

    //Level  *level;
    //Weapon *weapon;

    int  health;

    static int swingCounter;    // Used for 1st person swinging movement
    vec3 pos;
    vec3 rot;
    vec3 velocity;
    class Weapon* weapon;

    //Level *model; // it's may be full character or just gun model

    bool onGround;

    Player(/*Level *level,*/ PlayerType type);
    ~Player();

    void respawn();

    vec3 getHeadPos();
    vec3 getRot();

    void update();
    void collide();
    void trace(const vec3 &rayPos, const vec3 &rayDir, float &t);
};

#endif
