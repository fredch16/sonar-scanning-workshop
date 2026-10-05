/*
  GOAL
  Read and print the echo duration in microseconds.

  NEW IN THIS CHECKPOINT
  ECHO input, pulseIn(), Serial.begin(), Serial.println().

  UNLOCKED API
  See API.md in this folder.

  SUCCESS LOOKS LIKE
  Serial Monitor prints raw durations, or 0 on timeout.
*/
#include <Arduino.h>

const int TRIG_PIN = 25;
const int ECHO_PIN = 26;
const int SERVO_PIN = 27;

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  unsigned long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  Serial.println(duration);
  delay(100);
}
