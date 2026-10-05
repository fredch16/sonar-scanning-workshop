# 05_sweep

## Mission

Sweep from 10 to 170 degrees and back.

## New in this checkpoint

Two for loops for an automatic sweep.

## Available API and usage

APIs from earlier checkpoints remain available.

Use `for` loops: visit 10 through 170 in 2° steps, then 168 down through 12. The next forward pass visits 10, so endpoints are not duplicated. Command the angle and wait 60 ms at each step. Retain earlier wiring setup and the already introduced `measureDistance()` helper, but do not call it in this checkpoint. The loop only moves the servo and waits.

## Success condition

Servo sweeps continuously; distance measurement is not used in this level.
