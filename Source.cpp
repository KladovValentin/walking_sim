#pragma once

#include "MainHeader.h"
#include "functions_namespace.h"
#include "Object.h"
#include "Door.h"
#include "Wall.h"
#include "Room.h"
#include "Subject.h"


using namespace MyFunctions;

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
            if (dist < 10) {
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