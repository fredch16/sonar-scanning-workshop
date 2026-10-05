# 04_servo

## Mission

Command the servo to 30, 90, and 150 degrees.

## New in this checkpoint

ESP32Servo, attach(), write().

## Available API and usage

APIs from earlier checkpoints remain available.

Install ESP32Servo, include `<ESP32Servo.h>`, and create `Servo scannerServo;`. `scannerServo.attach(SERVO_PIN)` attaches its signal pin. `scannerServo.write(angle)` commands an angle in degrees. Use one-second delays between 30, 90, and 150. Keep the earlier standalone distance reading by calling `measureDistance()` after the manual servo sequence. Do not duplicate trigger/echo code. Invalid readings remain -1 cm.

## Success condition

Servo visits 30, 90, and 150 degrees repeatedly; distance still prints.
