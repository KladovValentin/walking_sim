#pragma once
#include "MainHeader.h"
#include "Wall.h"

//has corners, links to walls (NULL if no wall yet), objects inside and neighbour rooms (to search position after frame change only inside nearby rooms)
//convex is to search inside it, sfml just has usefull function "contains" - for example room contains hero.position
class Room
{
public:
    vector<point> corners;
    vector<Wall*> walls;
    vector<Room*> neighbours;
    sf::ConvexShape convex;
    static int n0;
    int n;

    Room(vector<point> inputcorners);
    void linkWall(int borderIndex, Wall* wall);
    //this linking is for "room[0]", "outside of a house"
    void linkWall0(Wall* wall);
    void addjust(Room* adjustedRoom);
};

