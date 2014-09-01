#include <iostream>
#include "libtcod.hpp"
#include "Actor.hpp"
#include "Engine.hpp"
#include "Map.hpp"

Engine engine;

int main()
{
    while (!TCODConsole::isWindowClosed())
    {
        engine.update();
        engine.render();
        TCODConsole::flush();
    }
    
    return 0;
}
