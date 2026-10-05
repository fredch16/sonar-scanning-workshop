/*
  GOAL
  Measure distance at every commanded angle.

  NEW IN THIS CHECKPOINT
  Servo settling followed by one ultrasonic reading.

  UNLOCKED API
  See API.md in this folder.

  SUCCESS LOOKS LIKE
  Serial Monitor shows labeled angle and distance readings.
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

  unsigned long duration = pulseIn(ECHO_PIN, HIGH, 5000);
  if (duration == 0) {
    return -1;  // No valid echo; this is not a zero-distance reading.
  }
  return duration * 0.0343f / 2.0f;  // Convert round-trip time to cm.
}

void scanAtAngle(int angle) {
  scannerServo.write(angle);
  delay(60);  // Let the servo settle; also spaces ultrasonic triggers.
  float distance = measureDistance();
  Serial.print("Angle: ");
  Serial.print(angle);
  Serial.print(" deg, distance: ");
  Serial.print(distance, 1);
  Serial.println(" cm");
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
    scanAtAngle(angle);
  }
  for (int angle = 168; angle >= 12; angle -= 2) {
    scanAtAngle(angle);
  }
}
