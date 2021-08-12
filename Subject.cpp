#include "Subject.h"

using namespace MyFunctions;

Subject::Subject(point startCoord, double basespeed) :Object(startCoord), baseSpeed(basespeed) {}

point Subject::pos() {
    return position;
}

void Subject::moves(point wheretogo, double movementMulty) {
    double localAngle = 0;
    if (wheretogo.x != 0)
        localAngle = atan(wheretogo.y / wheretogo.x);
    else
        localAngle = (double)(wheretogo.y > 0 ? 1 : -1) * 3.14159 / 2.;
    if (wheretogo.x < 0)
        localAngle = localAngle + 3.14159;
    double localMove = baseSpeed * movementMulty;
    position.x += localMove * cos(localAngle);
    position.y += localMove * sin(localAngle);
    setRotation(localAngle);
}

void Subject::checkBlockByWall(Wall* wall, point movement) {
    double angle = wall->angle;
    vector<double> startPoints;
    startPoints.push_back(0.);
    for (size_t i = 0; i < wall->doorway.size(); i++) {
        startPoints.push_back(wall->doorwaypars[i].x);
        startPoints.push_back(wall->doorwaypars[i].x + wall->doorwaypars[i].y);
    }
    startPoints.push_back(wall->lenght);

    for (int i = 0; i < wall->doorway.size() + 1; i++) {
        point vertexes[4];
        vertexes[0] = { wall->vertexes.first.x + wall->width / 2. * sin(angle) + startPoints[2 * i] * cos(angle), wall->vertexes.first.y - wall->width / 2. * cos(angle) + startPoints[2 * i] * sin(angle) };
        vertexes[1] = { wall->vertexes.first.x + wall->width / 2. * sin(angle) + startPoints[2 * i + 1] * cos(angle), wall->vertexes.first.y - wall->width / 2. * cos(angle) + startPoints[2 * i + 1] * sin(angle) };
        vertexes[2] = { wall->vertexes.first.x - wall->width / 2. * sin(angle) + startPoints[2 * i + 1] * cos(angle), wall->vertexes.first.y + wall->width / 2. * cos(angle) + startPoints[2 * i + 1] * sin(angle) };
        vertexes[3] = { wall->vertexes.first.x - wall->width / 2. * sin(angle) + startPoints[2 * i] * cos(angle), wall->vertexes.first.y + wall->width / 2. * cos(angle) + startPoints[2 * i] * sin(angle) };
        vector<pair<point, point>> temparr;
        for (size_t i = 0; i < 3; i++)
            temparr.push_back({ vertexes[i],vertexes[i + 1] });
        temparr.push_back({ vertexes[3],vertexes[0] });
        checkBlockByObject(temparr);
    }

    /*double cosWM = 0;
    if (movement.first != 0 || movement.second != 0) {
        cosWM = (wallv.first * movement.first + wallv.second * movement.second) / c / sqrt(pow(movement.first, 2) + pow(movement.second, 2));
    }*/
    /*point moveback = wallvp;
        if (movement.first != 0 || movement.second != 0) {
            moveback = { -movement.first,-movement.second };
            move({ wallv.first * cosWM,wallv.second * cosWM }, 1, -distToWall + sprite.getLocalBounds().width / 2. + 2.5);
        }
        move(moveback, 1, (-distToWall + sprite.getLocalBounds().width / 2. + 2.5) / sqrt(1 - pow(cosWM, 2)));*/
}

double Subject::MinDistanceToObj(vector<pair<point, point>> linevarr) {
    double minimalDistance = sprite.getLocalBounds().width / 2 + 500.;
    for (size_t i = 0; i < linevarr.size(); i++) {
        double tempdist = distanceToLine(linevarr[i], pos());
        if (tempdist < minimalDistance)
            minimalDistance = tempdist;
    }
    return minimalDistance - sprite.getLocalBounds().width / 2;
}

void Subject::checkBlockByObject(vector<pair<point, point>> linevarr) {
    double minimalDistance = sprite.getLocalBounds().width / 2 + 500.;
    size_t minimalDistInd = 0;
    int minimalDistInd2 = -1;
    for (size_t i = 0; i < linevarr.size(); i++) {
        double tempdist = distanceToLine(linevarr[i], pos());
        if (tempdist < minimalDistance) {
            minimalDistance = tempdist;
            minimalDistInd = i;
            minimalDistInd2 = -1;
        }
        if (tempdist == minimalDistance) {
            minimalDistInd2 = i;
        }
    }
    if (minimalDistance < sprite.getLocalBounds().width / 2.) {
        if (minimalDistInd2 != -1) {
            pushFromLine(linevarr[minimalDistInd], (sprite.getLocalBounds().width / 2. - minimalDistance) / 2.);
            pushFromLine(linevarr[minimalDistInd2], (sprite.getLocalBounds().width / 2. - minimalDistance) / 2.);
        }
        else
            pushFromLine(linevarr[minimalDistInd], sprite.getLocalBounds().width / 2. - minimalDistance);
    }
}

void Subject::pushFromLine(pair<point, point> linev, double distance) {
    point linev0 = { linev.second.x - linev.first.x , linev.second.y - linev.first.y };
    point linevp = { linev0.y , -linev0.x };  //on the left side
    move(linevp, distance);
}