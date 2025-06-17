#include "gameclock.h"
#include <chrono>

GameClock::GameClock()
{
    isPause = true;
    gameTime = 0;
    realTime = time_point_cast<milliseconds>(steady_clock::now());
}

int GameClock::getGameTime()
{
    if (isPause)
    {
        return gameTime;
    }
    else
    {
        return time_point_cast<milliseconds>(steady_clock::now()).time_since_epoch().count() - realTime.time_since_epoch().count() + gameTime;
    }
}

void GameClock::setGameTime(int time)
{
    gameTime = time;
    realTime = time_point_cast<milliseconds>(steady_clock::now());
}

void GameClock::pause()
{
    if (!isPause)
    {
        gameTime = getGameTime();
        isPause = true;
    }
}

void GameClock::resume()
{
    if (isPause)
    {
        realTime = time_point_cast<milliseconds>(steady_clock::now());
        isPause = false;
    }
}

void GameClock::reset()
{
    isPause = true;
    gameTime = 0;
    realTime = time_point_cast<milliseconds>(steady_clock::now());
}

bool GameClock::isRunning()
{
    return !isPause;
}
