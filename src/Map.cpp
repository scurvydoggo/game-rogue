#include "libtcod.hpp"
#include "Actor.hpp"
#include "Engine.hpp"
#include "Map.hpp"

static const int ROOM_MAX_SIZE = 12;
static const int ROOM_MIN_SIZE = 6;
static const int MAX_ROOM_MONSTERS = 3;

class BspListener : public ITCODBspCallback
{
private :
    Map &map;
    int roomNum;
    int lastX, lastY;
public :
    BspListener(Map &map) : map(map), roomNum(0) {}

    bool visitNode(TCODBsp *node, void *userData)
    {
        if (node->isLeaf())
        {
            // Dig a room
            int x, y, w, h;
            TCODRandom *rng = TCODRandom::getInstance();
            w = rng->getInt(ROOM_MIN_SIZE, node->w - 2);
            h = rng->getInt(ROOM_MIN_SIZE, node->h - 2);
            x = rng->getInt(node->x + 1, node->x + node->w - w - 1);
            y = rng->getInt(node->y + 1, node->y + node->h - h - 1);
            map.createRoom(roomNum == 0, x, y, x + w - 1, y + h - 1);

            if (roomNum != 0)
            {
                // Dig a corridor from the last room
                map.dig(lastX, lastY, x + w / 2, lastY);
                map.dig(x + w / 2, lastY, x + y / 2, y + h / 2);
            }

            lastX = x + w / 2;
            lastY = y + h / 2;
            roomNum++;
        }

        return true;
    }
};

Map::Map(int width, int height)
    : width(width), height(height)
{
    tiles = new Tile[width * height];
    map = new TCODMap(width, height);

    TCODBsp bsp(0, 0, width, height);
    bsp.splitRecursive(
        NULL,
        8,
        ROOM_MAX_SIZE,
        ROOM_MAX_SIZE,
        1.5f,
        1.5f);
    BspListener listener(*this);
    bsp.traverseInvertedLevelOrder(&listener, NULL);
}

Map::~Map()
{
    delete [] tiles;
    delete map;
}

bool Map::isWall(int x, int y) const
{
    return !map->isWalkable(x, y);
}

bool Map::canWalk(int x, int y) const
{
    if (isWall(x, y))
    {
        return false;
    }

    for (Actor **iterator = engine.actors.begin();
        iterator != engine.actors.end();
        iterator++)
    {
        Actor *actor = *iterator;
        if (actor->x == x && actor->y == y)
        {
            return false;
        }
    }

    return true;
}

bool Map::isExplored(int x, int y) const
{
    return tiles[x + y * width].isExplored;
}

bool Map::isInFov(int x, int y) const
{
    if (map->isInFov(x, y))
    {
        tiles[x + y * width].isExplored = true;
        return true;
    }
    
    return false;
}

void Map::computeFov()
{
    map->computeFov(
        engine.player->x,
        engine.player->y,
        engine.fovRadius);
}

void Map::dig(int x1, int y1, int x2, int y2)
{
    if (x2 < x1)
    {
        int tmp = x1;
        x1 = x2;
        x2 = tmp;
    }

    if (y2 < y1)
    {
        int tmp = y1;
        y1 = y2;
        y2 = tmp;
    }

    for (int x = x1; x <= x2; x++)
    {
        for (int y = y1; y <= y2; y++)
        {
            map->setProperties(x, y, true, true);
        }
    }
}

void Map::createRoom(bool first, int x1, int y1, int x2, int y2)
{
    dig (x1, y1, x2, y2);
    if (first)
    {
        // Put the player in the first room
        engine.player->x = (x1 + x2) / 2;
        engine.player->y = (y1 + y2) / 2;
    }
    else
    {
        TCODRandom *rng = TCODRandom::getInstance();
        int nbMonsters = rng->getInt(0, MAX_ROOM_MONSTERS);
        while (nbMonsters > 0)
        {
            int x = rng->getInt(x1, x2);
            int y = rng->getInt(y1, y2);
            if (canWalk(x, y))
            {
                addMonster(x, y);
            }
            nbMonsters--;
        }
    }
}
        
void Map::render() const
{
    static const TCODColor darkWall(0, 0, 100);
    static const TCODColor darkGround(50, 50, 150);
    static const TCODColor lightWall(130, 110, 50);
    static const TCODColor lightGround(200, 180, 50);

    TCODColor bg;
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            if (isInFov(x, y))
            {
                bg = isWall(x, y) ? lightWall : lightGround;
            }
            else
            {
                if (isWall(x, y) || !isExplored(x, y))
                {
                    bg = darkWall;
                }
                else
                {
                    bg = darkGround;
                }
            }

            TCODConsole::root->setCharBackground(x, y, bg);
        }
    }     
}

void Map::addMonster(int x, int y)
{
    TCODRandom *rng = TCODRandom::getInstance();
    Actor *monster;

    if (rng->getInt(0, 100) < 80)
    {
        monster = new Actor(
            x,
            y,
            'o',
            "orc",
            TCODColor::desaturatedGreen);
    }
    else
    {
        monster = new Actor(
            x,
            y,
            'T',
            "troll",
            TCODColor::darkerGreen);
    }

    engine.actors.push(monster);
}
