#ifndef TIMER_H_
#define TIMER_H_

/*

*/
class Timer
{
public:
    Timer();
    virtual ~Timer();

    void Start();
    void Pause();
    void Stop();
    void Begin();
    void BeginWithDelay(float delay);


    void SetTime(float f);
    float GetTime();
    bool isRunning();

    void Update(float fElapsedTime);


    bool m_isRuning;
    float m_Time;

};

#endif // TIMER_H_
