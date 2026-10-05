# 01_fire

## Mission

Generate a 10 microsecond trigger pulse.

## New in this checkpoint

pinMode(), digitalWrite(), delayMicroseconds(), delay().

## Available API and usage

APIs from earlier checkpoints remain available.

`pinMode(pin, OUTPUT)` configures TRIG. `digitalWrite(pin, LOW/HIGH)` changes its level. Hold LOW for 2 us, HIGH for 10 us, then LOW. `delayMicroseconds(us)` waits in microseconds; `delay(ms)` waits in milliseconds. Space triggers by at least 60 ms.

## Success condition

TRIG sends a pulse every 100 ms.
