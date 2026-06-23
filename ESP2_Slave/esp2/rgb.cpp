#include "rgb.h"
#include <Arduino.h>

void rgb_setup() {
  pinMode(RED_PIN,   OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN,  OUTPUT);
  rgb_stop();
}

void rgb_red() {
  digitalWrite(RED_PIN,   HIGH);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN,  LOW);
}
void rgb_yellow() {
  digitalWrite(RED_PIN,   HIGH);
  digitalWrite(GREEN_PIN, HIGH);
  digitalWrite(BLUE_PIN,  LOW);
}

void rgb_green() {
  digitalWrite(RED_PIN,   LOW);
  digitalWrite(GREEN_PIN, HIGH);
  digitalWrite(BLUE_PIN,  LOW);
}

void rgb_blue() {
  digitalWrite(RED_PIN,   LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN,  HIGH);
}

void rgb_stop() {
  digitalWrite(RED_PIN,   LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN,  LOW);
}