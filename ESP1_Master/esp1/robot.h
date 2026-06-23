#ifndef ROBOT_H
#define ROBOT_H

#include <Arduino.h>

enum Heading { NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3 };

void robot_init(int startX = 1, int startY = 2, Heading h = NORTH);
void robot_moveForward();
void robot_moveBackward();
void robot_turnRight();
void robot_turnLeft();
bool robot_execute(String cmd);

int     robot_getX();
int     robot_getY();
Heading robot_getHeading();

#endif
