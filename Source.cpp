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
    bool operator == (const point& rp) { return (x == rp.x && y == rp.y);}
};

struct vect {
    pair<point, point> vc;
};

//random functions
double distanceToLine(pair<point,point> linev, point p) {
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
    vertexes[0] = { centrPos.x + height / 2. * sin(angle) - width /2. * cos(angle), centrPos.y - height / 2. * cos(angle) - width / 2. * sin(angle) };
    vertexes[1] = { centrPos.x + height / 2. * sin(angle) + width /2. * cos(angle), centrPos.y - height / 2. * cos(angle) + width / 2. * sin(angle) };
    vertexes[2] = { centrPos.x - height / 2. * sin(angle) + width /2. * cos(angle), centrPos.y + height / 2. * cos(angle) + width / 2. * sin(angle) };
    vertexes[3] = { centrPos.x - height / 2. * sin(angle) - width /2. * cos(angle), centrPos.y + height / 2. * cos(angle) - width / 2. * sin(angle) };
    for (size_t i = 0; i < 3; i++)
        temp.push_back({ vertexes[i],vertexes[i + 1] });
    temp.push_back({ vertexes[3],vertexes[0] });
    return temp;
}

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

void Object::setRotation(double angle) {
    direction = angle;
    sprite.setRotation(direction * 180. / 3.14159);
    bindSpritePosition();
}
void Object::rotate(double angle) {
    direction = direction + angle;
    if (direction > 2. * 3.14159)
        direction = direction - 2. * 3.14159;
    sprite.setRotation(direction * 180. / 3.14159);
    bindSpritePosition();
}

void Object::bindSpritePosition() {
    double locWidth = sprite.getLocalBounds().width;
    double locHeight = sprite.getLocalBounds().height;
    double centreAngle = direction + atan(locHeight / locWidth);
    double diag = sqrt(pow(locWidth / 2., 2) + pow(locHeight / 2., 2));
    sprite.setPosition(position.x - diag * cos(centreAngle), position.y - diag * sin(centreAngle));
}

void Object::move(point wheretogo, double distance) {
    if (wheretogo.x != 0 || wheretogo.y != 0) {
        double localAngle = 0;
        if (wheretogo.x != 0)
            localAngle = atan(wheretogo.y / wheretogo.x);
        else
            localAngle = (double)(wheretogo.y > 0 ? 1 : -1) * 3.14159 / 2.;
        if (wheretogo.x < 0)
            localAngle = localAngle + 3.14159;
        position.x += distance * cos(localAngle);
        position.y += distance * sin(localAngle);
        bindSpritePosition();
    }
}

class Door : public Object {
public:
    bool locked;
    bool opened;

    Door(point startCoord) :Object(startCoord), locked(true),opened(false) {};
    bool isLocked() { return locked; }
    bool isOpened() { return opened; }
    void unlock() { locked = false; }
    void rotateDoor(double angle);
    void open() { 
        if (!isLocked()) {
            rotateDoor(3.14159 / 4.);
            opened = true;
        }
    }
    void close() {
        rotateDoor(-3.14159 / 4.);
        opened = false;
    }
};

void Door::rotateDoor(double angle) {
    double v1 = (sprite.getLocalBounds().width/2.-1) * cos(direction);
    double v2 = (sprite.getLocalBounds().width/2.-1) * sin(direction);
    position.x = position.x - v1 + v1 * cos(angle) - v2 * sin(angle);
    position.y = position.y - v2 + v1 * sin(angle) + v2 * cos(angle);
    rotate(angle);
}

//______________________different tipes of furniture_____________
//screen (returns video from youtube, or anime or from computer, maybe has interface)
//table, chair, shell (to wall), door (function - placeAtWall (Room,wallNumber))

//_________________________________wall side__________________________
class WallSide {
    pair<point, point> vertexes;
public:

};



//___________________________________wall_________________________________
class Wall
{
    //two vertexes of wall with pairs of coordinates
    double hp;  //0-demolished,100-full
public:
    pair<point, point> vertexes;
    sf::Vertex line[2];
    sf::RectangleShape rectangle;
    vector<point> doorwaypars;
    vector<sf::RectangleShape> doorway;
    double width;
    double angle;
    double lenght;

    //Constructor
    Wall(point v1, point v2);
    
    void changeHp(double damage);
    void makeDoorway(double startcoord, double doorsize); //in own frame of reference, coordinate from the begining of wall
    //TODO: make wall looks destroyed when it is destroed
    //TODO: create window with walls proportion when near it with xz coordinates? maybe not here
};

Wall::Wall(point v1, point v2) {
    vertexes.first = v1;
    vertexes.second = v2;
    hp = 100;
    width = 5;
    lenght = sqrt(pow(v2.x - v1.x,2)+ pow(v2.y - v1.y, 2));

    line[0] = sf::Vertex(sf::Vector2f(v1.x, v1.y));
    line[1] = sf::Vertex(sf::Vector2f(v2.x, v2.y));
    rectangle.setSize(sf::Vector2f(sqrt(pow(v2.x-v1.x,2)+ pow(v2.y - v1.y, 2)), width));
    if (v2.x - v1.x >= 0.01)
        angle = atan((v2.y - v1.y) / (v2.x - v1.x));
    else if (v2.x - v1.x <= -0.01)
        angle = 3.14159 + atan((v2.y - v1.y) / (v2.x - v1.x));
    else
        angle = 3.14159/2.*(double)(v2.y - v1.y > 0 ? 1 : -1);
    rectangle.rotate(angle * 180. / 3.14159);
    rectangle.setPosition(v1.x+width/2.*sin(angle),v1.y-width/2.*cos(angle));
}

void Wall::changeHp(double damage) {
    hp = hp + damage;
    if (hp < 0)
        hp = 0;
    if (hp > 100)
        hp = 100;
    cout << "new hp = " << hp << endl;
}

void Wall::makeDoorway(double startcoord, double doorsize) {
    sf::RectangleShape temp;
    temp.setSize(sf::Vector2f(doorsize, width));
    temp.rotate(angle*180./3.14159);
    temp.setPosition(vertexes.first.x + width / 2. * sin(angle) + startcoord * cos(angle), vertexes.first.y - width / 2. * cos(angle) + startcoord * sin(angle));
    sf::Color black = sf::Color::Black;
    temp.setFillColor(black);
    doorwaypars.push_back({ startcoord,doorsize });
    doorway.push_back(temp);
}

//______________________________room_____________________________________
class Room
{
public:
    vector<point> corners;
    vector<Wall*> walls;
    vector<Room*> neighbours;
    sf::ConvexShape convex;
    static int n0;
    int n;

    Room(vector<point> inputcorners);
    void linkWall(int borderIndex, Wall* wall);
    void linkWall0(Wall* wall);
    void addjust(Room *adjustedRoom);
};

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

//_______________________________subject________________________
class Subject: public Object
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
        startPoints.push_back(wall->doorwaypars[i].x+ wall->doorwaypars[i].y);
    }
    startPoints.push_back(wall->lenght);

    for (int i = 0; i < wall->doorway.size()+1; i++) {
        point vertexes[4];
        vertexes[0] = { wall->vertexes.first.x + wall->width / 2. * sin(angle) + startPoints[2*i] * cos(angle), wall->vertexes.first.y - wall->width / 2. * cos(angle) + startPoints[2*i] * sin(angle) };
        vertexes[1] = { wall->vertexes.first.x + wall->width / 2. * sin(angle) + startPoints[2*i+1] * cos(angle), wall->vertexes.first.y - wall->width / 2. * cos(angle) + startPoints[2*i+1] * sin(angle) };
        vertexes[2] = { wall->vertexes.first.x - wall->width / 2. * sin(angle) + startPoints[2*i+1] * cos(angle), wall->vertexes.first.y + wall->width / 2. * cos(angle) + startPoints[2*i+1] * sin(angle) };
        vertexes[3] = { wall->vertexes.first.x - wall->width / 2. * sin(angle) + startPoints[2*i] * cos(angle), wall->vertexes.first.y + wall->width / 2. * cos(angle) + startPoints[2*i] * sin(angle) };
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
            pushFromLine(linevarr[minimalDistInd], (sprite.getLocalBounds().width / 2. - minimalDistance)/2.);
            pushFromLine(linevarr[minimalDistInd2], (sprite.getLocalBounds().width / 2. - minimalDistance)/2.);
        }
        else
            pushFromLine(linevarr[minimalDistInd], sprite.getLocalBounds().width / 2. - minimalDistance);
    }
}

void Subject::pushFromLine(pair<point, point> linev, double distance) {
    point linev0 = { linev.second.x - linev.first.x , linev.second.y - linev.first.y };
    point linevp = { linev0.y , -linev0.x };  //on the left side
    move(linevp,distance);
}



//__________________________________main_________________________________
int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "window");
    window.setFramerateLimit(60);

    sf::RenderWindow window1(sf::VideoMode(800, 400), "window");
    window1.setFramerateLimit(60);
    window1.setPosition(sf::Vector2i(10, 10));

    vector<Room> rooms;
    rooms.push_back(Room({ {0,0},{800,0},{800,800},{0,800} }));
    rooms.push_back(Room({ {10,10},{110,10},{110,110},{10,110} }));
    rooms.push_back(Room({ {110,10},{210,10},{250,110},{210,210},{110,110} }));
    rooms[1].addjust(&rooms[2]);
    rooms[2].addjust(&rooms[1]);
    
    //constructing and linking walls
    vector<Wall> walls;
    {
        walls.push_back(Wall({ 10,10 }, { 110,10 }));
        walls.push_back(Wall({ 110,10 }, { 110,110 }));
        walls.push_back(Wall({ 110,110 }, { 10,110 }));
        walls.push_back(Wall({ 10,110 }, { 10,10 }));

        walls.push_back(Wall({ 110,10 }, { 210,10 }));
        walls.push_back(Wall({ 210,10 }, { 250,110 }));
        walls.push_back(Wall({ 250,110 }, { 210,210 }));
        walls.push_back(Wall({ 210,210 }, { 110,110 }));

        rooms[1].linkWall(0, &walls[0]);
        rooms[1].linkWall(1, &walls[1]);
        rooms[1].linkWall(2, &walls[2]);
        rooms[1].linkWall(3, &walls[3]);

        rooms[2].linkWall(0, &walls[4]);
        rooms[2].linkWall(1, &walls[5]);
        rooms[2].linkWall(2, &walls[6]);
        rooms[2].linkWall(3, &walls[7]);
        rooms[2].linkWall(4, rooms[1].walls[1]);

        for(size_t j = 0; j < walls.size(); j++)
            rooms[0].linkWall0(&walls[j]);
    }

    walls[1].makeDoorway(35,30);

    Door door({ 110,60 });
    sf::Texture doorTexture;
    doorTexture.loadFromFile("images/door.png");
    door.sprite.setTexture(doorTexture);
    door.setRotation(walls[1].angle);
    door.bindSpritePosition();
    door.unlock();
    //door.open();

    Subject hero({ 140,40 }, 100);
    sf::Texture heroTexture;
    heroTexture.loadFromFile("images/sword2.png");
    hero.sprite.setTexture(heroTexture);
    hero.bindSpritePosition();

    int currentRoomMouse = 1;
    int currentRoomHero = 1;
    sf::Event prevEvent;
    prevEvent.type = sf::Event::MouseButtonReleased;
    sf::Clock clock;
    sf::Clock eclock;
    point move = { 0,0 };
    double dt = 1;
    double etime = 0;
    while (window.isOpen())
    {
        sf::Vector2i pos = sf::Mouse::getPosition(window);
        
        //in what room we are, need to write some about neighbors
        currentRoomMouse = 0;
        for (size_t i = 1; i < rooms.size(); i++) {
            if (rooms[i].convex.getGlobalBounds().contains(pos.x, pos.y)) {
                currentRoomMouse = i;
            }
        }
        currentRoomHero = 0;
        for (size_t i = 1; i < rooms.size(); i++) {
            if (rooms[i].convex.getGlobalBounds().contains(hero.pos().x, hero.pos().y)) {
                currentRoomHero = i;
            }
        }

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        sf::Time frameTime = clock.restart();
        dt = frameTime.asSeconds();

        if (event.type == sf::Event::MouseButtonPressed)
            if (event.key.code == sf::Mouse::Left) {}

        if (event.type == sf::Event::MouseButtonReleased && prevEvent.type == sf::Event::MouseButtonPressed)
            if (event.key.code == sf::Mouse::Left) {
                for (size_t i = 0; i < rooms[currentRoomMouse].walls.size(); i++) {
                    if (rooms[currentRoomMouse].walls[i]->rectangle.getGlobalBounds().contains(pos.x, pos.y))
                    {
                        //change hp of this wall and adjacent one from another room
                        rooms[currentRoomMouse].walls[i]->changeHp(-10);
                        if (rooms[currentRoomMouse].neighbours[i] != NULL) {
                            //for (size_t j = 0; j < rooms[rooms[currentRoom].neighbours[i]->n].walls.size(); j++)
                            //     if (rooms[rooms[currentRoom].neighbours[i]->n].neighbours[j] == &rooms[currentRoom])
                            //         rooms[currentRoom].neighbours[i]->walls[j]->changeHp(-10);
                        }
                    }
                }
            }


        move = { 0,0 };
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            move.y -= 1.;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            move.y += 1.;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            move.x -= 1.;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            move.x += 1.;
        if (!(move.x == 0 && move.y == 0)) {
            hero.moves(move, dt);
        }

        //checkWalls();
        for (size_t i = 0; i < rooms[currentRoomHero].walls.size(); i++) {
            if (rooms[currentRoomHero].walls[i] != NULL) {
                hero.checkBlockByWall(rooms[currentRoomHero].walls[i], move);
            }
        }
        hero.checkBlockByObject(bordersOfRectangle(door.position,door.sprite.getLocalBounds().width,door.sprite.getLocalBounds().height,door.direction));

        sf::Time coolDown = eclock.getElapsedTime();
        etime = coolDown.asSeconds();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && (etime>1)) {
            eclock.restart();
            window1.close();
            double dist = hero.MinDistanceToObj(bordersOfRectangle(door.position, door.sprite.getLocalBounds().width, door.sprite.getLocalBounds().height, door.direction));
            if (dist < 50) {
                if (door.isOpened())
                    door.close();
                else
                    door.open();
            }
        }

        //new window with changing size depended on distance to door
        /*double dist = hero.MinDistanceToObj(bordersOfRectangle(door.position, door.sprite.getLocalBounds().width, door.sprite.getLocalBounds().height, door.direction));
        if (dist < 25) {
            if (!window1.isOpen()) {
                window1.create(sf::VideoMode((int)(400 - (dist * 10)), (int)(200 - (dist * 5))), "window1");
                window1.setPosition(sf::Vector2i(10, 10));
            }
            window1.setSize(sf::Vector2u((int)(400 - (dist * 10)), (int)(200 - (dist * 5))));
        }
        else
            window1.close();*/

        prevEvent = event;
        window.clear();
        for (size_t j = 0; j < walls.size(); j++) {
            window.draw(walls[j].rectangle);
            for (size_t d = 0; d < walls[j].doorway.size(); d++)
                window.draw(walls[j].doorway[d]);
        }
        window.draw(door.sprite);
        window.draw(hero.sprite);
        window.display();

        window1.clear();
        window1.draw(door.sprite);
        window1.draw(hero.sprite);
        window1.display();
    }

    /*while (window1.isOpen()) {
        sf::Event event;
        while (window1.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window1.close();
        }
    }*/


    return 0;
}



// and also hold the side of wall (from which side if you look from the start of the wall`s vector))

// construct walls: first create them with the usage of function that returns vectors of room's borders and then link (push_back or change from NULL (no wall)) to the room
// if the wall exist with this coordinates - then dont create another but link it with room. Also I can create and link them "manually"
// wall sides can be another class that also has a vector of coordinates, and it should be a part of the wall, I think