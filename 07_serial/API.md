# 07_serial

## Mission

Send one angle,distance pair per line to Processing.

## New in this checkpoint

Comma-separated serial output.

## Available API and usage

APIs from earlier checkpoints remain available.

`Serial.print(angle)`, `Serial.print(',')`, and `Serial.println(distance, 1)` produce exactly two numeric fields per line. Print no labels, headers, or startup messages. Use 115200 baud and close Serial Monitor before opening Processing. -1.0 means no valid echo within 5 ms. Keep the same `measureDistance()` and `scanAtAngle()` helpers; change only the printing to this numeric protocol.

## Success condition

Each line looks like 90,23.4 with no headings or labels.
