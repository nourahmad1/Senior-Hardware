#include "motors.h"
#include <Arduino.h>

static void _drive(bool a1, bool a2, bool b1, bool b2, int spd) {
    ledcWrite(MOTOR_ENA, spd);
    ledcWrite(MOTOR_ENB, spd);
    digitalWrite(MOTOR_IN1, a1 ? HIGH : LOW);
    digitalWrite(MOTOR_IN2, a2 ? HIGH : LOW);
    digitalWrite(MOTOR_IN3, b1 ? HIGH : LOW);
    digitalWrite(MOTOR_IN4, b2 ? HIGH : LOW);
}

// Stop only Motor A (ENA / IN1 / IN2)
static void _stopA() {
    ledcWrite(MOTOR_ENA, 0);
    digitalWrite(MOTOR_IN1, LOW);
    digitalWrite(MOTOR_IN2, LOW);
}

// Stop only Motor B (ENB / IN3 / IN4)
static void _stopB() {
    ledcWrite(MOTOR_ENB, 0);
    digitalWrite(MOTOR_IN3, LOW);
    digitalWrite(MOTOR_IN4, LOW);
}

// Drives both motors together, but stops Motor A after baseMs and keeps
// Motor B running for an extra MOTOR_B_EXTRA_MS to compensate for it
// being weaker/slower (prevents drift during straight-line driving).
static void _driveCompensated(bool a1, bool a2, bool b1, bool b2, int spd, unsigned long baseMs) {
    _drive(a1, a2, b1, b2, spd);
    delay(baseMs);
    _stopB();
    delay(MOTOR_B_EXTRA_MS);
    _stopA();
}

void motors_setup() {
    pinMode(MOTOR_IN1, OUTPUT);
    pinMode(MOTOR_IN2, OUTPUT);
    pinMode(MOTOR_IN3, OUTPUT);
     pinMode(MOTOR_IN4, OUTPUT);
    ledcAttach(MOTOR_ENA, 5000, 8);
    ledcAttach(MOTOR_ENB, 5000, 8);
    motors_stop();
    Serial.println("MOTORS READY");
}

void motors_stop() {
    _drive(false, false, false, false, 0);
}

void motors_forward_one_cell() {
    Serial.println("MOTOR: forward");
    _driveCompensated(true, false, true, false, MOTOR_SPEED, CELL_FORWARD_MS);
}

void motors_backward_one_cell() {
    Serial.println("MOTOR: backward");
    _driveCompensated(false, true, false, true, MOTOR_SPEED, CELL_BACKWARD_MS);
}

void motors_turn_right_90() {
    Serial.println("MOTOR: turn right");
    _drive(true, false, false, true, MOTOR_TURN_SPEED);
    delay(TURN_90_MS);
    motors_stop();
}

void motors_turn_left_90() {
    Serial.println("MOTOR: turn left");
    _drive(false, true, true, false, MOTOR_TURN_SPEED);
    delay(TURN_90_MS);
    motors_stop();
}
