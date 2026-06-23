#include "position.h"
#include "motors.h"
#include "commands.h"
#include <Arduino.h>

// Expected sensor readings [row][col] = {front, back, left, right} in cm
// Track: 120x140cm, Car: 15x26cm, sensors at car edges, outer walls only
static const float EXPECTED[4][4][4] = {
    {{ 4.5f,109.5f, 7.5f,97.5f},{ 4.5f,109.5f,37.5f,67.5f},{ 4.5f,109.5f,67.5f,37.5f},{ 4.5f,109.5f,97.5f, 7.5f}},
    {{39.5f, 74.5f, 7.5f,97.5f},{39.5f, 74.5f,37.5f,67.5f},{39.5f, 74.5f,67.5f,37.5f},{39.5f, 74.5f,97.5f, 7.5f}},
    {{74.5f, 39.5f, 7.5f,97.5f},{74.5f, 39.5f,37.5f,67.5f},{74.5f, 39.5f,67.5f,37.5f},{74.5f, 39.5f,97.5f, 7.5f}},
    {{109.5f, 4.5f, 7.5f,97.5f},{109.5f, 4.5f,37.5f,67.5f},{109.5f, 4.5f,67.5f,37.5f},{109.5f, 4.5f,97.5f, 7.5f}}
};

extern volatile SensorPacket latestSensors;
extern volatile bool         sensorsReady;
extern bool requestSensors(uint32_t timeoutMs);

static void _nudgeForward() {
    digitalWrite(MOTOR_IN1, HIGH); digitalWrite(MOTOR_IN2, LOW);
    digitalWrite(MOTOR_IN3, HIGH); digitalWrite(MOTOR_IN4, LOW);
    ledcWrite(MOTOR_ENA, MOTOR_SPEED); ledcWrite(MOTOR_ENB, MOTOR_SPEED);
    delay(POS_NUDGE_MS); motors_stop(); delay(50);
}
static void _nudgeBackward() {
    digitalWrite(MOTOR_IN1, LOW);  digitalWrite(MOTOR_IN2, HIGH);
    digitalWrite(MOTOR_IN3, LOW);  digitalWrite(MOTOR_IN4, HIGH);
    ledcWrite(MOTOR_ENA, MOTOR_SPEED); ledcWrite(MOTOR_ENB, MOTOR_SPEED);
    delay(POS_NUDGE_MS); motors_stop(); delay(50);
}
static void _nudgeRight() {
    digitalWrite(MOTOR_IN1, HIGH); digitalWrite(MOTOR_IN2, LOW);
    digitalWrite(MOTOR_IN3, LOW);  digitalWrite(MOTOR_IN4, HIGH);
    ledcWrite(MOTOR_ENA, MOTOR_SPEED); ledcWrite(MOTOR_ENB, MOTOR_SPEED);
    delay(POS_NUDGE_MS / 2); motors_stop(); delay(50);
}
static void _nudgeLeft() {
    digitalWrite(MOTOR_IN1, LOW);  digitalWrite(MOTOR_IN2, HIGH);
    digitalWrite(MOTOR_IN3, HIGH); digitalWrite(MOTOR_IN4, LOW);
    ledcWrite(MOTOR_ENA, MOTOR_SPEED); ledcWrite(MOTOR_ENB, MOTOR_SPEED);
    delay(POS_NUDGE_MS / 2); motors_stop(); delay(50);
}

bool position_verify(int col, int row) {
    if (col < 0 || col > 3 || row < 0 || row > 3) return false;

    float expF = EXPECTED[row][col][0];
    float expB = EXPECTED[row][col][1];
    float expL = EXPECTED[row][col][2];
    float expR = EXPECTED[row][col][3];

    for (int i = 0; i < POS_MAX_ATTEMPTS; i++) {
        if (!requestSensors(400)) { Serial.println("POS: sensor timeout"); return false; }

        float front = latestSensors.d1;
        float back  = latestSensors.d2;
        float left  = latestSensors.d3;
        float right = latestSensors.d4;

        Serial.printf("POS[%d] F:%.1f(%.1f) B:%.1f(%.1f) L:%.1f(%.1f) R:%.1f(%.1f)\n",
            i, front, expF, back, expB, left, expL, right, expR);

        float fbErr = (front > 0) ? (front - expF) : (expB - back);
        float lrErr = (left  > 0) ? (left  - expL) : (expR - right);

        bool fbOk = fabsf(fbErr) <= POS_TOLERANCE_CM;
        bool lrOk = fabsf(lrErr) <= POS_TOLERANCE_CM;

        if (fbOk && lrOk) { Serial.printf("POS: OK in %d steps\n", i+1); return true; }

        if (!fbOk) { fbErr > 0 ? _nudgeForward() : _nudgeBackward(); continue; }
        if (!lrOk) { lrErr > 0 ? _nudgeLeft()    : _nudgeRight();    }
    }

    Serial.println("POS: failed to correct");
    return false;
}
