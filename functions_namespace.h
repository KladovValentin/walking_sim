#pragma once
#include "MainHeader.h"

namespace MyFunctions
{
    double distanceToLine(pair<point, point> linev, point p);

    vector<pair<point, point>> bordersOfRectangle(point centrPos, double width, double height, double angle);

}