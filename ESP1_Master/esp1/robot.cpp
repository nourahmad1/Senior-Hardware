#include "robot.h"
#include "motors.h"
#include "screen.h"
#include <Arduino.h>

static int     _x = 1, _y = 2;
static Heading _heading = NORTH;

void robot_init(int startX, int startY, Heading h) {
    _x = startX; _y = startY; _heading = h;
    Serial.printf("ROBOT INIT pos=(%d,%d) heading=%d\n", _x, _y, _heading);
}

int     robot_getX()       { return _x; }
int     robot_getY()       { return _y; }
Heading robot_getHeading() { return _heading; }

static void _updatePos(Heading dir) {
    switch (dir) {
        case NORTH: _y = max(0, _y - 1); break;
        case SOUTH: _y = min(3, _y + 1); break;
        case WEST:  _x = max(0, _x - 1); break;
        case EAST:  _x = min(3, _x + 1); break;
    }
}

static void _afterMove() {
    screen_showHappy();
    delay(800);
    screen_setup();
}

void robot_moveForward() {
    motors_forward_one_cell();
    _updatePos(_heading);
    _afterMove();
}

void robot_moveBackward() {
    Heading back = (Heading)((_heading + 2) % 4);
    motors_backward_one_cell();
    _updatePos(back);
    _afterMove();
}

void robot_turnRight() {
    _heading = (Heading)((_heading + 1) % 4);
    motors_turn_right_90();
    delay(800);
}

void robot_turnLeft() {
    _heading = (Heading)((_heading + 3) % 4);
    motors_turn_left_90();
    delay(800);
}

bool robot_execute(String cmd) {
    if      (cmd == "MF") { robot_moveForward();  return true; }
    else if (cmd == "MB") { robot_moveBackward(); return true; }
    else if (cmd == "TR") { robot_turnRight();    return true; }
    else if (cmd == "TL") { robot_turnLeft();     return true; }
    return false;
}
