# Ultrasonic scanner: complete stage-by-stage API reference

This document covers every checkpoint from `00_starter` to `08_final`. Follow the stages in order; APIs introduced in earlier stages remain available later. Each checkpoint sketch is independently compilable. Stage 05 practices sweeping without taking distance readings; stage 06 combines the two skills.

## Contents

- [00_starter](#00_starter)
- [01_fire](#01_fire)
- [02_listen](#02_listen)
- [03_distance](#03_distance)
- [04_servo](#04_servo)
- [05_sweep](#05_sweep)
- [06_scan](#06_scan)
- [07_serial](#07_serial)
- [08_final](#08_final)

## Setup and wiring shared by all stages

In Arduino IDE, install **esp32 by Espressif Systems** through Boards Manager and select **ESP32 Dev Module**. From stage 04 onward, install **ESP32Servo by Kevin Harrington and John K. Bennett** through Library Manager. The include is case-sensitive: `#include <ESP32Servo.h>`. PlatformIO dependencies are separate from Arduino IDE installations.

| Connection | ESP32 connection |
| --- | --- |
| HC-SR04 VCC | 5V |
| HC-SR04 GND | GND |
| HC-SR04 TRIG | GPIO 25 |
| HC-SR04 ECHO | GPIO 26 through the divider below |
| Servo signal | GPIO 27 |
| Servo power | Suitable 5V supply |
| Servo ground | Common GND |

```text
HC-SR04 ECHO --- 1 kΩ ---+--- GPIO 26
                        |
                      1.8 kΩ
                        |
                       GND
```

The divider reduces the 5 V ECHO signal to approximately 3.2 V. All grounds must be connected together, including an external servo supply ground if used. The ESP32 3.3V power pin is unused. Servo wire colors are usually red for power, brown/black for ground, and orange/yellow/white for signal; check your servo's markings.

## API quick reference

| API or construct | Parameters and behavior | First stage |
| --- | --- | --- |
| `#include <Arduino.h>` | Arduino definitions; used in every sketch | 00 |
| `const int PIN = number;` | Declare a fixed GPIO number | 00 |
| `void setup()` | Runs once after startup/reset | 00 |
| `void loop()` | Repeats after setup finishes | 00 |
| `Serial.begin(115200)` | Start serial; baud rate must match the receiver | 02 |
| `pinMode(pin, OUTPUT)` | Configure an output pin | 01 |
| `digitalWrite(pin, LOW/HIGH)` | Set the output level | 01 |
| `delayMicroseconds(us)` | Block for a number of microseconds | 01 |
| `delay(ms)` | Block for a number of milliseconds | 01 |
| `pinMode(pin, INPUT)` | Configure an input pin | 02 |
| `pulseIn(pin, HIGH, timeoutUs)` | Return HIGH pulse duration in µs as `unsigned long`; return 0 on timeout | 02 |
| `Serial.println(value)` | Print a value followed by a newline | 02 |
| `float` | Store fractional distance values | 03 |
| `Serial.println(distance, 1)` | Print a float with one decimal place and a newline | 03 |
| `#include <ESP32Servo.h>` | Include the separately installed servo library | 04 |
| `Servo scannerServo;` | Create a servo object | 04 |
| `scannerServo.attach(pin)` | Attach the servo signal to a GPIO | 04 |
| `scannerServo.write(angle)` | Command an angle in degrees | 04 |
| `for (initialization; condition; update)` | Repeat a block while its condition is true | 05 |
| `float measureDistance()` | Trigger, read echo, return centimeters or -1 for no valid echo | 03 |
| `void scanAtAngle(int angle)` | Workshop helper combining movement and measurement; returns no value | 06 |
| `Serial.print(value)` | Print without a newline, allowing a CSV line to be assembled | 06 |
| `Serial.print(',')` | Print the comma separating angle and distance | 07 |

The 10 µs trigger stays the same throughout. Stages 02–04 use a 30000 µs echo timeout; stages 06–08 use 15000 µs for faster scanning. Raw `pulseIn()` returning 0 means no valid echo. From stage 03 onward, `measureDistance()` returns -1 for that case; distance output is -1.0, never a timeout represented as 0 cm.

## 00_starter

### Mission

Declare the wiring pins in an ESP32 project skeleton.

### New in this checkpoint

ESP32 project skeleton and pin constants only.

### Available API and usage

APIs from earlier checkpoints remain available.

Declare TRIG_PIN = 25, ECHO_PIN = 26, and SERVO_PIN = 27; leave setup() and loop() empty.

### Success condition

Compiles and uploads successfully.

## 01_fire

### Mission

Generate a 10 microsecond trigger pulse.

### New in this checkpoint

pinMode(), digitalWrite(), delayMicroseconds(), delay().

### Available API and usage

APIs from earlier checkpoints remain available.

`pinMode(pin, OUTPUT)` configures TRIG. `digitalWrite(pin, LOW/HIGH)` changes its level. Hold LOW for 2 us, HIGH for 10 us, then LOW. `delayMicroseconds(us)` waits in microseconds; `delay(ms)` waits in milliseconds. Space triggers by at least 60 ms.

### Success condition

TRIG sends a pulse every 100 ms.

## 02_listen

### Mission

Read and print the echo duration in microseconds.

### New in this checkpoint

ECHO input, pulseIn(), Serial.begin(), Serial.println().

### Available API and usage

APIs from earlier checkpoints remain available.

`Serial.begin(115200)` starts serial in setup(); set Serial Monitor to 115200 baud. `pinMode(ECHO_PIN, INPUT)` enables echo input. `pulseIn(ECHO_PIN, HIGH, 30000)` returns the HIGH pulse duration as unsigned long microseconds, or 0 if it times out. Print it with `Serial.println(duration)`. Wire ECHO through the divider described in the README.

### Success condition

Serial Monitor prints raw durations, or 0 on timeout.

## 03_distance

### Mission

Convert echo travel time to distance in centimeters.

### New in this checkpoint

A reusable function, distance conversion, and invalid-echo handling.

### Available API and usage

APIs from earlier checkpoints remain available.

Sound travels approximately 0.0343 cm/us at room temperature. Use `float distance = duration * 0.0343f / 2.0f;` because the echo travels out and back. `Serial.println(distance, 1)` prints one decimal place. Put the trigger pulse, `pulseIn()`, and conversion inside `float measureDistance()`. If `duration == 0`, use `return -1;` before conversion. Otherwise return the converted distance. In `loop()`, use `float distance = measureDistance();` and print it. A function returning `float` supplies a fractional numeric result to its caller; `return` finishes the call.

### Success condition

Serial Monitor prints distance in cm, or -1.0 on timeout.

## 04_servo

### Mission

Command the servo to 30, 90, and 150 degrees.

### New in this checkpoint

ESP32Servo, attach(), write().

### Available API and usage

APIs from earlier checkpoints remain available.

Install ESP32Servo, include `<ESP32Servo.h>`, and create `Servo scannerServo;`. `scannerServo.attach(SERVO_PIN)` attaches its signal pin. `scannerServo.write(angle)` commands an angle in degrees. Use one-second delays between 30, 90, and 150. Keep the earlier standalone distance reading by calling `measureDistance()` after the manual servo sequence. Do not duplicate trigger/echo code. Invalid readings remain -1 cm.

### Success condition

Servo visits 30, 90, and 150 degrees repeatedly; distance still prints.

## 05_sweep

### Mission

Sweep from 10 to 170 degrees and back.

### New in this checkpoint

Two for loops for an automatic sweep.

### Available API and usage

APIs from earlier checkpoints remain available.

Use `for` loops: visit 10 through 170 in 2° steps, then 168 down through 12. The next forward pass visits 10, so endpoints are not duplicated. Command the angle and wait 60 ms at each step. Retain earlier wiring setup and the already introduced `measureDistance()` helper, but do not call it in this checkpoint. The loop only moves the servo and waits.

### Success condition

Servo sweeps continuously; distance measurement is not used in this level.

## 06_scan

### Mission

Measure distance at every commanded angle.

### New in this checkpoint

Servo settling followed by one ultrasonic reading.

### Available API and usage

APIs from earlier checkpoints remain available.

Create `void scanAtAngle(int angle)` to command the servo, wait 60 ms, call `measureDistance()`, and print labeled readings. Keep trigger/echo logic inside `measureDistance()` only. Call it once at each angle in both sweep loops.

### Success condition

Serial Monitor shows labeled angle and distance readings.

Use 2° steps and reduce the timeout inside `measureDistance()` to `pulseIn(ECHO_PIN, HIGH, 15000)`. The timeout limits the measurable range to approximately 257 cm; timeouts return -1 cm.

## 07_serial

### Mission

Send one angle,distance pair per line to Processing.

### New in this checkpoint

Comma-separated serial output.

### Available API and usage

APIs from earlier checkpoints remain available.

`Serial.print(angle)`, `Serial.print(',')`, and `Serial.println(distance, 1)` produce exactly two numeric fields per line. Print no labels, headers, or startup messages. Use 115200 baud and close Serial Monitor before opening Processing. -1.0 means no valid echo within 15 ms. Keep the same `measureDistance()` and `scanAtAngle()` helpers; change only the printing to this numeric protocol.

### Success condition

Each line looks like 90,23.4 with no headings or labels.

## 08_final

### Mission

Use the complete readable reference. This stage introduces no new functionality.

### Final code structure

1. Pin constants: TRIG on GPIO 25, ECHO on GPIO 26, servo on GPIO 27.
2. `Servo scannerServo`: the only object needed beyond Arduino serial.
3. `float measureDistance()`: send the 10 µs trigger, call `pulseIn(ECHO_PIN, HIGH, 15000)`, return -1 if duration is 0, otherwise return `duration * 0.0343f / 2.0f` in cm.
4. `void scanAtAngle(int angle)`: move the servo, wait 60 ms, call `measureDistance()`, then print `angle,distance` followed by a newline.
5. `setup()`: start serial at 115200 baud, configure TRIG and ECHO, attach the servo, command 10°, and wait 500 ms for the initial move.
6. `loop()`: simple forward and reverse `for` loops calling `scanAtAngle()`.

### Success condition

Every commanded angle produces exactly one numeric line, such as `90,23.4`. A timeout produces `90,-1.0`. Print no header or labels. Receivers should skip plotting invalid distances.

### Timing and sweep

The forward loop visits 10° through 170° in 2° steps; the reverse visits 168° through 12°. The next cycle starts at 10°, avoiding repeated endpoints. There are 160 readings per full cycle, taking approximately 9.6–12 seconds plus small execution and serial overhead. The 15 ms timeout corresponds to approximately 257 cm. Angles are commanded positions; the servo provides no position feedback.

The reference uses blocking delays and simple functions. It needs no `millis()`, state machines, enums, or additional classes.
