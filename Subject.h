#pragma once
#include "Object.h"
#include "Wall.h"
#include "functions_namespace.h"

class Subject :
    public Object
{
    double baseSpeed;
public:

    Subject(point startCoord, double basespeed);

    point pos();
    void moves(point wheretogo, double movementMulty);
    void pushFromLine(pair<point, point> linev, double distance);
    double MinDistanceToObj(vector<pair<point, point>> linevarr);
    void checkBlockByObject(vector<pair<point, point>> linevarr);
    void checkBlockByWall(Wall* wall, point movement);
};

