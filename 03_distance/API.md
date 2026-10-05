# 03_distance

## Mission

Convert echo travel time to distance in centimeters.

## New in this checkpoint

A reusable function, distance conversion, and invalid-echo handling.

## Available API and usage

APIs from earlier checkpoints remain available.

Sound travels approximately 0.0343 cm/us at room temperature. Use `float distance = duration * 0.0343f / 2.0f;` because the echo travels out and back. `Serial.println(distance, 1)` prints one decimal place. Put the trigger pulse, `pulseIn()`, and conversion inside `float measureDistance()`. If `duration == 0`, use `return -1;` before conversion. Otherwise return the converted distance. In `loop()`, use `float distance = measureDistance();` and print it. A function returning `float` supplies a fractional numeric result to its caller; `return` finishes the call.

## Success condition

Serial Monitor prints distance in cm, or -1.0 on timeout.
