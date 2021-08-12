#include "Door.h"

void Door::rotateDoor(double angle) {
    double v1 = (sprite.getLocalBounds().width / 2. - 1) * cos(direction);
    double v2 = (sprite.getLocalBounds().width / 2. - 1) * sin(direction);
    position.x = position.x - v1 + v1 * cos(angle) - v2 * sin(angle);
    position.y = position.y - v2 + v1 * sin(angle) + v2 * cos(angle);
    rotate(angle);
}