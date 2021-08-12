#pragma once
#include "MainHeader.h"
#include "Wall.h"

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
    void linkWall0(Wall* wall);
    void addjust(Room* adjustedRoom);
};

