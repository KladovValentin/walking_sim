#pragma once
#include "Object.h"
#include "Wall.h"
#include "functions_namespace.h"

//movable by itself object
class Subject :
    public Object
{
    double baseSpeed;
public:

    Subject(point startCoord, double basespeed);

    point pos();
    
    //movement in derection with some speed (based on baseSpeed)
    void moves(point wheretogo, double movementMulty);
    
    //pushing in normal to line direction, so direction of lines is important! (clockwise around object)
    void pushFromLine(pair<point, point> linev, double distance);
   
    double MinDistanceToObj(vector<pair<point, point>> linevarr);
    
    void checkBlockByObject(vector<pair<point, point>> linevarr);
    
    //mostly create an array to put into checkBlockByObject
    void checkBlockByWall(Wall* wall, point movement);
};

