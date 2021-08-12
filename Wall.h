#pragma once
#include "MainHeader.h"

class Wall
{
    //two vertexes of wall with pairs of coordinates
    double hp;  //0-demolished,100-full
public:
    pair<point, point> vertexes;
    sf::Vertex line[2];
    sf::RectangleShape rectangle;
    vector<point> doorwaypars;
    vector<sf::RectangleShape> doorway;
    double width;
    double angle;
    double lenght;

    //Constructor
    Wall(point v1, point v2);

    void changeHp(double damage);
    void makeDoorway(double startcoord, double doorsize); //in own frame of reference, coordinate from the begining of wall
    //TODO: make wall looks destroyed when it is destroed
    //TODO: create window with walls proportion when near it with xz coordinates? maybe not here
};

