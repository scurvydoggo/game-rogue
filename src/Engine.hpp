#ifndef ROGUE_ENGINE_HPP
#define ROGUE_ENGINE_HPP

#include "libtcod.hpp"
#include "Actor.hpp"
#include "Map.hpp"

class Engine
{
public :
    enum GameStatus
    {
        STARTUP,
        IDLE,
        NEW_TURN,
        VICTORY,
        DEFEAT
    } gameStatus;

    TCODList<Actor *> actors;
    Actor *player;
    Map *map;
    int fovRadius;

    Engine();
    ~Engine();
    void update();
    void render();
};

extern Engine engine;

#endif
