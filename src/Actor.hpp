#ifndef ROGUE_ACTOR_H
#define ROGUE_ACTOR_H

#include "libtcod.hpp"

class Actor
{
public :
    int x, y;
    int ch;
    TCODColor col;
    const char *name;
    
    Actor(int x, int y, int ch, const char *name, const TCODColor &col);
    void update();
    void render() const;
    bool moveOrAttack(int x, int y);
};

#endif
