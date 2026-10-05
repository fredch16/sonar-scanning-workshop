# ESP32 ultrasonic scanner workshop

Build a servo-mounted HC-SR04 scanner one idea at a time. Each checkpoint is a complete known-good sketch participants can open when they get stuck. Only the HC-SR04 and servo are controlled by the code.

## Hardware and wiring

Use an ESP32 Dev Module, HC-SR04, a 5 V hobby servo, jumper wires, and two resistors for ECHO voltage protection.

| Connection | ESP32 |
| --- | --- |
| HC-SR04 VCC | 5V |
| HC-SR04 GND | GND |
| HC-SR04 TRIG | GPIO 25 |
| HC-SR04 ECHO | GPIO 26 through the divider below |
| Servo signal | GPIO 27 |
| Servo power | 5V supply appropriate for the servo |
| Servo ground | GND, shared with ESP32 and HC-SR04 |

GPIO 25, 26, and 27 are available on the workshop board and support the required functions. GPIO 12 is avoided because it is a boot strapping pin; GPIO 34 and 35 are input-only. The 3.3V pin is not used for powering these 5 V components.

A standard 5 V HC-SR04 has a 5 V ECHO output. Do not connect it directly to an ESP32 input. Add this divider (for example, 1 kΩ and 1.8 kΩ), giving approximately 3.2 V:

```text
HC-SR04 ECHO --- 1 kΩ ---+--- GPIO 26
                        |
                      1.8 kΩ
                        |
                       GND
```

The divider is passive wiring protection, not another programmed workshop component. Power the servo from a 5 V source with sufficient current for its specified load; a weak USB/board supply can cause resets. With a separate servo supply, connect its ground to ESP32 ground and keep its positive supply on the servo power connection.

## Arduino IDE setup

1. Install the **esp32 by Espressif Systems** board package and select **ESP32 Dev Module**.
2. For checkpoint 04 onward, install **ESP32Servo** through Library Manager.
3. Open `<checkpoint>/<checkpoint>.ino`, select the serial port, and upload. Every folder is an independent sketch.
4. From checkpoint 02 onward, set Serial Monitor to **115200 baud**.

## Checkpoints

Each folder contains its working `.ino` and an `API.md` with the mission, newly introduced APIs, and success condition. Try the mission first and use the sketch to catch up. The combined guide is [API_REFERENCE.md](API_REFERENCE.md).

| Checkpoint | Working state |
| --- | --- |
| [00_starter](00_starter/API.md) | ESP32 project skeleton and pin constants only |
| [01_fire](01_fire/API.md) | 10 µs trigger pulse; no echo reading |
| [02_listen](02_listen/API.md) | Raw echo duration in microseconds |
| [03_distance](03_distance/API.md) | Distance in cm using speed of sound and /2 |
| [04_servo](04_servo/API.md) | Manual commands to 30, 90, 150; earlier distance reading retained |
| [05_sweep](05_sweep/API.md) | Sweep from 10° to 170° and back; no distance reading |
| [06_scan](06_scan/API.md) | One distance reading at each commanded angle |
| [07_serial](07_serial/API.md) | Numeric angle,distance output for Processing |
| [08_final](08_final/API.md) | Clean final reference with the same behavior as 07 |

These sketches use blocking delays and simple loops. Checkpoints 05–08 sweep in 2° steps. The scanner waits 60 ms after commanding each angle before measuring. A full forward-and-back sweep takes roughly 9.6–10.4 seconds instead of 25.6–35.2 seconds, depending on echoes. Actual servo positioning depends on the servo and mounting; increase that delay if necessary. The return loop skips duplicated endpoints.

Checkpoints 02–04 use a 30 ms `pulseIn()` timeout. Scanning checkpoints 06–08 use 5 ms, limiting the measurable range to approximately 86 cm; timeouts return -1 cm. Distance uses 0.0343 cm/µs and divides the round-trip travel by two. From checkpoint 03 onward, `measureDistance()` returns -1 when `pulseIn()` returns 0; serial distance output is then -1.0. Checkpoint 02 prints raw duration only, so its 0 means no valid echo and is not a distance.

## PlatformIO

The default `pio run` builds `08_final`. Build or upload any checkpoint independently:

```sh
pio run -e 02_listen
pio run -e 07_serial -t upload
pio device monitor -b 115200
```

Small source wrappers include each Arduino sketch. The source filter selects only the requested checkpoint; ESP32Servo is required only from checkpoint 04 onward. `src/main.cpp` also points to the final reference for tools that expect the conventional source entrypoint.

## Processing serial contract

Checkpoints 07 and 08 emit exactly `angle,distance` with one measurement per line, no header, labels, or startup messages:

```text
10,23.4
11,23.6
12,-1.0
```

Angle is an integer in degrees; distance is centimeters with one decimal place. Distance -1.0 means invalid/no echo; the receiver should skip plotting that measurement. Configure the Processing receiver for 115200 baud, newline termination, and comma-separated fields. Close Serial Monitor before opening the receiver. No Processing sketch is included in this checkpoint task.

## Hardware references

- [Espressif GPIO documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/gpio.html)
- [Espressif voltage limits and GPIO guidance](https://docs.espressif.com/projects/esp-faq/en/latest/hardware-related/hardware-design.html)
- [HC-SR04 product documentation](https://www.sparkfun.com/ultrasonic-distance-sensor-hc-sr04.html)
- [ESP32Servo source and API](https://github.com/madhephaestus/ESP32Servo)

The Processing display covers 0–50 cm. The scan echo timeout is 5 ms (approximately 86 cm), providing headroom for the 4.37 ms round-trip echo at 75 cm. Readings beyond 50 cm are not plotted.
