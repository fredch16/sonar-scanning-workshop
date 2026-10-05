# 06_scan

## Mission

Measure distance at every commanded angle.

## New in this checkpoint

Servo settling followed by one ultrasonic reading.

## Available API and usage

APIs from earlier checkpoints remain available.

Create `void scanAtAngle(int angle)` to command the servo, wait 60 ms, call `measureDistance()`, and print labeled readings. Keep trigger/echo logic inside `measureDistance()` only. Call it once at each angle in both sweep loops.

## Success condition

Serial Monitor shows labeled angle and distance readings.

Use 2° steps and reduce the timeout inside `measureDistance()` to `pulseIn(ECHO_PIN, HIGH, 5000)`. The timeout limits the measurable range to approximately 86 cm; timeouts return -1 cm.
