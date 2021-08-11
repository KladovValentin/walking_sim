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


//______________________furniture____________________________
class furniture {
    //TODO
};

//______________________different tipes of furniture_____________
//screen (returns video from youtube, or anime or from computer, maybe has interface)
//table, chair, shell (to wall), door (function - placeAtWall (Room,wallNumber))

//___________________________________wall_________________________________
class Wall
{
    //two vertexes of wall with pairs of coordinates
    double hp;  //0-demolished,100-full
public:
    std::pair<std::pair<double, double>, std::pair<double, double>> vertexes;
    sf::Vertex line[2];
    sf::RectangleShape rectangle;
    //Constructor
    Wall(std::pair<double, double> v1, std::pair<double, double> v2);
    void changeHp(double damage);

    //TODO: make wall looks destroyed when it is destroed
    //TODO: create window with walls proportion when near it with xz coordinates? maybe not here
};

Wall::Wall(std::pair<double, double> v1, std::pair<double, double> v2) {
    vertexes.first = v1;
    vertexes.second = v2;
    hp = 100;
    line[0] = sf::Vertex(sf::Vector2f(v1.first, v1.second));
    line[1] = sf::Vertex(sf::Vector2f(v2.first, v2.second));
    rectangle.setSize(sf::Vector2f(sqrt(pow(v2.first-v1.first,2)+ pow(v2.second - v1.second, 2)), 5));
    double angle = 0;
    if (v2.first - v1.first >= 0.01)
        angle = asin((v2.second - v1.second) / (v2.first - v1.first)) * 180. / 3.14159;
    else if (v2.first - v1.first <= -0.01)
        angle = 180. - asin((v2.second - v1.second) / (v2.first - v1.first)) * 180. / 3.14159;
    else
        angle = 90*(double)(v2.second - v1.second > 0 ? 1 : -1);
    rectangle.rotate(angle);
    rectangle.setPosition(v1.first+2.5*sin(angle),v1.second-2.5*cos(angle));
}

void Wall::changeHp(double damage) {
    hp = hp + damage;
    if (hp < 0)
        hp = 0;
    if (hp > 100)
        hp = 100;
    std::cout << "new hp = " << hp << std::endl;
}

//______________________________room_____________________________________
class Room
{
public:
    std::vector<Wall> walls;
    std::vector<std::pair<double, double>> corners;
    std::vector<Room*> neighbours;
    sf::ConvexShape convex;
    static int n0;
    int n;

    Room(std::vector<std::pair<double, double>> inputcorners);
    void constructWall(int borderIndex);
    void addjust(Room *adjustedRoom);
};

Room::Room(std::vector<std::pair<double, double>> inputcorners) {
    convex.setPointCount(inputcorners.size());
    for (size_t i = 0; i < inputcorners.size(); i++) {
        corners.push_back(inputcorners[i]);
        convex.setPoint(i, sf::Vector2f(inputcorners[i].first, inputcorners[i].second));
    }
    for (size_t i = 0; i < inputcorners.size(); i++) {
        neighbours.push_back(NULL);
    }
    corners.push_back(inputcorners[0]);
    n = n0;
    n0++;
}

int Room::n0 = 0;

void Room::constructWall(int borderIndex) {
    walls.push_back(Wall(corners[borderIndex], corners[borderIndex + 1]));
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
class Subject
{
    std::pair<double, double> position;
    double direction; //angle
    double baseSpeed;
public:
    sf::Sprite sprite;

    Subject(std::pair<double, double> startCoord);
    std::pair<double, double> pos();
    void rotate(double angle); //radians, clockwise

    void bindSpritePosition();
    void move(std::pair<double, double> wheretogo, double movementMulty);
    void move(std::pair<double, double> wheretogo, double movementMulty, double distance);
    void checkBlockByWall(Wall wall, std::pair<double, double> move);
};

Subject::Subject(std::pair<double, double> startCoord) :position(startCoord), direction(0.), baseSpeed(350) {}

std::pair<double, double> Subject::pos() {
    return position;
}

void Subject::rotate(double angle) {
    direction = angle;
    sprite.setRotation(direction*180./3.14159);
    bindSpritePosition();
}

void Subject::bindSpritePosition() {
    double locWidth = sprite.getLocalBounds().width;
    double locHeight = sprite.getLocalBounds().height;
    double centreAngle = direction + atan(locHeight / locWidth);
    double diag = sqrt(pow(locWidth / 2., 2)+pow(locHeight / 2., 2));
    sprite.setPosition(position.first - diag * cos(centreAngle), position.second - diag * sin(centreAngle));
}

void Subject::move(std::pair<double, double> wheretogo, double movementMulty) {
    double localAngle = 0;
    if (wheretogo.first != 0)
        localAngle = atan(wheretogo.second / wheretogo.first);
    else
        localAngle = (double)(wheretogo.second > 0 ? 1 : -1) * 3.14159 / 2.;
    if (wheretogo.first < 0)
        localAngle = localAngle + 3.14159;
    double localMove = baseSpeed * movementMulty;
    position.first += localMove * cos(localAngle);
    position.second += localMove * sin(localAngle);
    rotate(localAngle);
}

void Subject::move(std::pair<double, double> wheretogo, double movementMulty, double distance) {
    double localAngle = 0;
    if (wheretogo.first != 0)
        localAngle = atan(wheretogo.second / wheretogo.first);
    else
        localAngle = (double)(wheretogo.second > 0 ? 1 : -1) * 3.14159 / 2.;
    if (wheretogo.first < 0)
        localAngle = localAngle + 3.14159;
    position.first += distance * cos(localAngle);
    position.second += distance * sin(localAngle);
    bindSpritePosition();
}

void Subject::checkBlockByWall(Wall wall, std::pair<double, double> movement) {
    double a = sqrt(pow(wall.vertexes.first.first - pos().first, 2) + pow(wall.vertexes.first.second - pos().second, 2));
    double b = sqrt(pow(wall.vertexes.second.first - pos().first, 2) + pow(wall.vertexes.second.second - pos().second, 2));
    double c = sqrt(pow(wall.vertexes.first.first - wall.vertexes.second.first, 2) + pow(wall.vertexes.first.second - wall.vertexes.second.second, 2));
    std::pair<double, double> wallv = { wall.vertexes.second.first - wall.vertexes.first.first , wall.vertexes.second.second - wall.vertexes.first.second };
    std::pair<double, double> wallvp = { -wallv.second , wallv.first };
    double distToWall = sqrt(2. * (pow(a, 2) + pow(b, 2)) - (pow((pow(a, 2) - pow(b, 2)) / c, 2)) - pow(c, 2)) / 2.;
    double cosWM = 0;
    if (movement.first != 0 && movement.second != 0)
        cosWM = (wallv.first * movement.first + wallv.second * movement.second) / c / sqrt(pow(movement.first, 2) + pow(movement.second, 2));
    if (distToWall < sprite.getLocalBounds().width / 2. + 2.5) {
        std::pair<double, double> moveback = wallvp;
        if (movement.first != 0 && movement.second != 0) {
            moveback = { -movement.first,-movement.second };
            move({ wallv.first * cosWM,wallv.second * cosWM }, 1, -distToWall + sprite.getLocalBounds().width / 2. + 2.5);
        }
        move(moveback, 1, (-distToWall + sprite.getLocalBounds().width / 2. + 2.5) / sqrt(1 - pow(cosWM, 2)));
    }
}

//__________________________________main_________________________________
int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "window");
    
    std::vector<Room> rooms;
    rooms.push_back(Room({ {0,0},{800,0},{800,800},{0,800} }));
    rooms.push_back(Room({ {10,10},{110,10},{110,110},{10,110} }));
    rooms.push_back(Room({ {110,10},{210,10},{210,110},{110,110} }));
    rooms[1].addjust(&rooms[2]);
    rooms[2].addjust(&rooms[1]);
    for (size_t j = 1; j < rooms.size(); j++) {
        for (size_t i = 0; i < rooms[j].corners.size() - 1; i++) {
            rooms[j].constructWall(i);
        }
    }

    Subject hero({ 40,40 });
    sf::Texture heroTexture;
    heroTexture.loadFromFile("images/sword1.png");
    hero.sprite.setTexture(heroTexture);
    hero.bindSpritePosition();

    int currentRoom = 1;
    int currentRoomHero = 1;
    sf::Event prevEvent;
    prevEvent.type = sf::Event::MouseButtonReleased;
    sf::Clock clock;
    while (window.isOpen())
    {
        sf::Vector2i pos = sf::Mouse::getPosition(window);
        
        //in what room we are
        /*
            for (size_t i = 0; i < rooms[currentRoom].neighbours.size(); i++)
                if (rooms[currentRoom].neighbours[i].first->convex.getGlobalBounds().contains(pos.x, pos.y))
                    currentRoom = rooms[currentRoom].neighbours[i].first->n;*/
        currentRoom = 0;
        for (size_t i = 1; i < rooms.size(); i++) {
            if (rooms[i].convex.getGlobalBounds().contains(pos.x, pos.y)) {
                currentRoom = i;
            }
        }
        currentRoomHero = 0;
        for (size_t i = 1; i < rooms.size(); i++) {
            if (rooms[i].convex.getGlobalBounds().contains(hero.pos().first, hero.pos().second)) {
                currentRoomHero = i;
            }
        }

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        sf::Time frameTime{ clock.restart() };
        double dt{ frameTime.asSeconds() };

        if (event.type == sf::Event::MouseButtonPressed)
            if (event.key.code == sf::Mouse::Left) {}
                
        if (event.type == sf::Event::MouseButtonReleased && prevEvent.type == sf::Event::MouseButtonPressed)
            if (event.key.code == sf::Mouse::Left) {
                for (size_t i = 0; i < rooms[currentRoom].walls.size(); i++) {
                    if (rooms[currentRoom].walls[i].rectangle.getGlobalBounds().contains(pos.x, pos.y))
                    {
                        //change hp of this wall and adjacent one from another room
                        rooms[currentRoom].walls[i].changeHp(-10);
                        if (rooms[currentRoom].neighbours[i] != NULL) {
                            
                            for (size_t j = 0; j < rooms[rooms[currentRoom].neighbours[i]->n].walls.size(); j++)
                                if (rooms[rooms[currentRoom].neighbours[i]->n].neighbours[j] == &rooms[currentRoom])
                                    rooms[currentRoom].neighbours[i]->walls[j].changeHp(-10);
                        }
                    }
                }
            }
                

        std::pair<double, double> move = {0,0};
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            move.second-=1.;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            move.second+=1.;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            move.first -=1.;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            move.first +=1.;
        if (!(move.first == 0 && move.second == 0)){
            hero.move(move,dt);
        }

        //checkWalls();
        for (size_t i = 0; i < rooms[currentRoomHero].walls.size(); i++) {
            hero.checkBlockByWall(rooms[currentRoomHero].walls[i],move);
        }
        

        prevEvent = event;
        window.clear();
        for (size_t j = 1; j < rooms.size(); j++) {
            for (size_t i = 0; i < rooms[j].corners.size() - 1; i++) {
                window.draw(rooms[j].walls[i].rectangle);
            }
        }
        window.draw(hero.sprite);
        window.display();
    }

    return 0;
}