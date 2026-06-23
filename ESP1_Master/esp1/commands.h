#ifndef COMMANDS_H
#define COMMANDS_H

// Original — untouched
typedef struct
{
    char cmd[16];
} CommandPacket;

// New — added for sensor data ESP2 → ESP1
typedef struct {
    float d1;  // FRONT
    float d2;  // BACK
    float d3;  // LEFT
    float d4;  // RIGHT
} SensorPacket;

#endif
