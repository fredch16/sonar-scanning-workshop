/*
  GOAL
  Convert echo travel time to distance in centimeters.

  NEW IN THIS CHECKPOINT
  measureDistance(), distance conversion, and -1 for an invalid echo.

  UNLOCKED API
  See API.md in this folder.

  SUCCESS LOOKS LIKE
  Serial Monitor prints distance in cm, or -1 on timeout.
*/
#include <Arduino.h>

const int TRIG_PIN = 25;
const int ECHO_PIN = 26;
const int SERVO_PIN = 27;

float measureDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration == 0) {
    return -1;  // No valid echo; this is not a zero-distance reading.
  }
  return duration * 0.0343f / 2.0f;  // Convert round-trip time to cm.
}

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  float distance = measureDistance();
  Serial.println(distance, 1);
  delay(100);
}
