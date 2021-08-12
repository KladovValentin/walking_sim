#pragma once

#include "MainHeader.h"
#include "Object.h"

class Door : public Object {
public:
    bool locked;
    bool opened;

    Door(point startCoord) :Object(startCoord), locked(true), opened(false) {};
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