#pragma once

#include "MainHeader.h"


//______________________furniture____________________________
class Object {
    //TODO
public:
    double direction;
    point position;
    sf::Sprite sprite;

    Object(point startCoord) : position(startCoord), direction(0.) {};

    void setRotation(double angle); //radians, clockwise
    void rotate(double angle); //radians, clockwise

    void bindSpritePosition();
    void move(point wheretogo, double distance);
};


//______________________furniture____________________________


//______________________different tipes of furniture_____________
//screen (returns video from youtube, or anime or from computer, maybe has interface)
//table, chair, shell (to wall), door (function - placeAtWall (Room,wallNumber))