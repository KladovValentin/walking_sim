//here all used libraries are declared and structs. All .cpp and .h files have to include it (can be done not directly)
#pragma once

#include <fstream> 
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <cmath>
#include <SFML/Graphics.hpp>

using namespace std;

struct point {
    double x;
    double y;
    bool operator == (const point& rp) { return (x == rp.x && y == rp.y); }
};

struct vect {
    pair<point, point> vc;
};
