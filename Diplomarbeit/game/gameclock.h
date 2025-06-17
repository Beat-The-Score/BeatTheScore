#ifndef GAMECLOCK_H
#define GAMECLOCK_H

#include <chrono>

using namespace std::chrono;

class GameClock
{
public:
    GameClock();
    int getGameTime();
    void setGameTime(int time);
    void pause();
    void resume();
    void reset();

    bool isRunning();

private:
    bool isPause;
    time_point<steady_clock, milliseconds> realTime;
    int gameTime;
};

#endif // GAMECLOCK_H
