/*
  GOAL
  Command the servo to 30, 90, and 150 degrees.

  NEW IN THIS CHECKPOINT
  ESP32Servo, attach(), write().

  UNLOCKED API
  See API.md in this folder.

  SUCCESS LOOKS LIKE
  Servo visits 30, 90, and 150 degrees repeatedly; distance still prints.
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
}

void loop() {
  scannerServo.write(30);
  delay(1000);
  scannerServo.write(90);
  delay(1000);
  scannerServo.write(150);
  delay(1000);

  float distance = measureDistance();
  Serial.println(distance, 1);
  delay(100);
}
