#ifndef ROGUE_ENGINE_HPP
#define ROGUE_ENGINE_HPP

#include "libtcod.hpp"
#include "Actor.hpp"
#include "Map.hpp"

class Engine
{
public :
    TCODList<Actor *> actors;
    Actor *player;
    Map *map;

    Engine();
    ~Engine();
    void update();
    void render();
};

extern Engine engine;

#endif
