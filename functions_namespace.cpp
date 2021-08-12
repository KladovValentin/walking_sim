#include "functions_namespace.h"

namespace MyFunctions
{
    double distanceToLine(pair<point, point> linev, point p) {
        point av = { linev.first.x - p.x, linev.first.y - p.y };
        point bv = { linev.second.x - p.x, linev.second.y - p.y };
        point wallv = { linev.second.x - linev.first.x , linev.second.y - linev.first.y };
        double a = sqrt(pow(av.x, 2) + pow(av.y, 2));
        double b = sqrt(pow(bv.x, 2) + pow(bv.y, 2));
        double c = sqrt(pow(wallv.x, 2) + pow(wallv.y, 2));
        if (wallv.x * av.x + wallv.y * av.y > 0)
            return a;
        if (wallv.x * bv.x + wallv.y * bv.y < 0)
            return b;
        return sqrt(2. * (pow(a, 2) + pow(b, 2)) - (pow((pow(a, 2) - pow(b, 2)) / c, 2)) - pow(c, 2)) / 2.;
    }

    vector<pair<point, point>> bordersOfRectangle(point centrPos, double width, double height, double angle) {
        vector<pair<point, point>> temp;
        point vertexes[4];
        vertexes[0] = { centrPos.x + height / 2. * sin(angle) - width / 2. * cos(angle), centrPos.y - height / 2. * cos(angle) - width / 2. * sin(angle) };
        vertexes[1] = { centrPos.x + height / 2. * sin(angle) + width / 2. * cos(angle), centrPos.y - height / 2. * cos(angle) + width / 2. * sin(angle) };
        vertexes[2] = { centrPos.x - height / 2. * sin(angle) + width / 2. * cos(angle), centrPos.y + height / 2. * cos(angle) + width / 2. * sin(angle) };
        vertexes[3] = { centrPos.x - height / 2. * sin(angle) - width / 2. * cos(angle), centrPos.y + height / 2. * cos(angle) - width / 2. * sin(angle) };
        for (size_t i = 0; i < 3; i++)
            temp.push_back({ vertexes[i],vertexes[i + 1] });
        temp.push_back({ vertexes[3],vertexes[0] });
        return temp;
    }
}