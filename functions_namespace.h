#pragma once
#include "MainHeader.h"


// I have some functions that is used by different classes like "cout"
// The best way to implement them is to use namespaces 
namespace MyFunctions
{
    //from point to line with 2 coordinates
    double distanceToLine(pair<point, point> linev, point p);

    //create an array of "physical" vectors of rectangle-shaped object's borders, clockwise starting from left-up courner of unrotated (angle=0) object
    vector<pair<point, point>> bordersOfRectangle(point centrPos, double width, double height, double angle);

}
