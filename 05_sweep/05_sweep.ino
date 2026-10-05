/*
  GOAL
  Sweep from 10 to 170 degrees and back.

  NEW IN THIS CHECKPOINT
  Two for loops for an automatic sweep.

  UNLOCKED API
  See API.md in this folder.

  SUCCESS LOOKS LIKE
  Servo sweeps continuously; distance measurement is not used in this level.
*/
#include <Arduino.h>
#include <ESP32Servo.h>

const int TRIG_PIN = 25;
const int ECHO_PIN = 26;
const int SERVO_PIN = 27;

Servo scannerServo;

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
  scannerServo.attach(SERVO_PIN);
  scannerServo.write(10);
  delay(500);  // Allow the initial move to finish.
}

void loop() {
  for (int angle = 10; angle <= 170; angle += 2) {
    scannerServo.write(angle);
    delay(60);
  }
  for (int angle = 168; angle >= 12; angle -= 2) {
    scannerServo.write(angle);
    delay(60);
  }
}
