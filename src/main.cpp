#include <iostream>
#include "libtcod.hpp"
using namespace std;

int main()
{
    TCODConsole::setCustomFont(
        "resources/font.bmp",
        TCOD_FONT_LAYOUT_ASCII_INROW,
        16,
        16);
    TCODConsole::initRoot(80, 50, "Rogue", false);
    while (!TCODConsole::isWindowClosed())
    {
        TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, NULL, NULL);
        TCODConsole::root->clear();
        TCODConsole::root->putChar(40, 25, '@');
        TCODConsole::flush();
    }
    
    return 0;
}
