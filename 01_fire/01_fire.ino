/*
  GOAL
  Generate a 10 microsecond trigger pulse.

  NEW IN THIS CHECKPOINT
  pinMode(), digitalWrite(), delayMicroseconds(), delay().

  UNLOCKED API
  See API.md in this folder.

  SUCCESS LOOKS LIKE
  TRIG sends a pulse every 100 ms.
*/
#include <Arduino.h>

const int TRIG_PIN = 25;
const int ECHO_PIN = 26;
const int SERVO_PIN = 27;

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);
}

void loop() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  delay(100);
}
