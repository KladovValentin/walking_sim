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
    
    //radians, clockwise, setting for sprite (sfml-image) and internal class value (direction). 0 ~ xy(1,0) (to the right)
    void setRotation(double angle); 
    void rotate(double angle); //radians, clockwise
    
    //placing sprite as such that point "position" is located at its centre (recalculating since sprite internal sfml position is left-up corner)
    void bindSpritePosition();
    //move on certain distance and bind sprite
    void move(point wheretogo, double distance);
};


//______________________furniture____________________________


//______________________different tipes of furniture_____________
//screen (returns video from youtube, or anime or from computer, maybe has interface)
//table, chair, shell (to wall), door (function - placeAtWall (Room,wallNumber))
