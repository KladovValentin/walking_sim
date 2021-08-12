#include "Room.h"


Room::Room(vector<point> inputcorners) {
    convex.setPointCount(inputcorners.size());
    for (size_t i = 0; i < inputcorners.size(); i++) {
        corners.push_back(inputcorners[i]);
        convex.setPoint(i, sf::Vector2f(inputcorners[i].x, inputcorners[i].y));
    }
    for (size_t i = 0; i < inputcorners.size(); i++) {
        neighbours.push_back(NULL);
        walls.push_back(NULL);
    }
    corners.push_back(inputcorners[0]);
    n = n0;
    n0++;
}

int Room::n0 = 0;

void Room::linkWall(int borderIndex, Wall* wall) {
    walls[borderIndex] = wall;
}

void Room::linkWall0(Wall* wall) {
    walls.push_back(wall);
}

void Room::addjust(Room* room2) {
    for (size_t i = 0; i < corners.size() - 1; i++) {
        for (size_t j = 1; j < room2->corners.size(); j++) {
            if (corners[i] == room2->corners[j] && corners[i + 1] == room2->corners[j - 1]) {
                neighbours[i] = room2;
            }
        }
    }
}