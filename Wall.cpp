#include "Wall.h"

Wall::Wall(point v1, point v2) {
    vertexes.first = v1;
    vertexes.second = v2;
    hp = 100;
    width = 5;
    lenght = sqrt(pow(v2.x - v1.x, 2) + pow(v2.y - v1.y, 2));

    line[0] = sf::Vertex(sf::Vector2f(v1.x, v1.y));
    line[1] = sf::Vertex(sf::Vector2f(v2.x, v2.y));
    rectangle.setSize(sf::Vector2f(sqrt(pow(v2.x - v1.x, 2) + pow(v2.y - v1.y, 2)), width));
    if (v2.x - v1.x >= 0.01)
        angle = atan((v2.y - v1.y) / (v2.x - v1.x));
    else if (v2.x - v1.x <= -0.01)
        angle = 3.14159 + atan((v2.y - v1.y) / (v2.x - v1.x));
    else
        angle = 3.14159 / 2. * (double)(v2.y - v1.y > 0 ? 1 : -1);
    rectangle.rotate(angle * 180. / 3.14159);
    rectangle.setPosition(v1.x + width / 2. * sin(angle), v1.y - width / 2. * cos(angle));
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
    temp.rotate(angle * 180. / 3.14159);
    temp.setPosition(vertexes.first.x + width / 2. * sin(angle) + startcoord * cos(angle), vertexes.first.y - width / 2. * cos(angle) + startcoord * sin(angle));
    sf::Color black = sf::Color::Black;
    temp.setFillColor(black);
    doorwaypars.push_back({ startcoord,doorsize });
    doorway.push_back(temp);
}