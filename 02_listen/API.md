# 02_listen

## Mission

Read and print the echo duration in microseconds.

## New in this checkpoint

ECHO input, pulseIn(), Serial.begin(), Serial.println().

## Available API and usage

APIs from earlier checkpoints remain available.

`Serial.begin(115200)` starts serial in setup(); set Serial Monitor to 115200 baud. `pinMode(ECHO_PIN, INPUT)` enables echo input. `pulseIn(ECHO_PIN, HIGH, 30000)` returns the HIGH pulse duration as unsigned long microseconds, or 0 if it times out. Print it with `Serial.println(duration)`. Wire ECHO through the divider described in the README.

## Success condition

Serial Monitor prints raw durations, or 0 on timeout.
