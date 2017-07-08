
#ifndef CK2_TIMER_H
#define CK2_TIMER_H


namespace CipherKick {

class Timer
{

private:

    // __int64 performanceFrequency;
    // __int64 performanceCountStart;

public:

    Timer();

    void start();

    long elapsedTimeMillis();

};

}


#endif //CK2_TIMER_H
