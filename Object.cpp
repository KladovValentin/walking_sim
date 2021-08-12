#include "Object.h"

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