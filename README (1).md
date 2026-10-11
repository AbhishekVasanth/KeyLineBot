# KeyLineBot

An Arduino-based line-following robot that also reads a binary pattern placed along its path and stops when the pattern matches a code typed on a keypad.

Built as a team project (4 members) for the Microprocessors and Microcontrollers course (BECE204L) at VIT Chennai, November 2025.

## What it does
- Follows a black line on a white surface using two front IR sensors.
- Reads a 0/1 pattern from the side using two more IR sensors (one acts as a clock, one as data).
- Keeps the latest 4 bits and compares them with a number (0-15) entered on a 4x4 keypad.
- Stops when the pattern matches and prints "MATCH FOUND" on the Serial Monitor.

## How the line following works
| Left sensor | Right sensor | Action |
|---|---|---|
| White | White | Move forward |
| Black | White | Turn left |
| White | Black | Turn right |
| Black | Black | Stop |

The motors run at different PWM speeds (left 132, right 180) and move in short steps (300 ms move, 300 ms pause) so the side sensors have time to read the pattern.

## Components
- Arduino Uno
- L293D motor driver
- 2 BO motors with wheels and a caster wheel
- 2 IR line sensors (front)
- 2 IR sensors (side: clock and data)
- 4x4 keypad
- 7.4 V Li-ion battery

## Pin connections
| Component | Arduino pin |
|---|---|
| Motor driver ENA | 5 |
| Motor driver ENB | 6 |
| Motor driver IN1 | 2 |
| Motor driver IN2 | 3 |
| Motor driver IN3 | 4 |
| Motor driver IN4 | 7 |
| Left line sensor | 8 |
| Right line sensor | 9 |
| Left side IR (clock) | 10 |
| Right side IR (data) | 11 |
| Keypad rows R1-R4 | A0-A3 |
| Keypad columns C1-C4 | A4, A5, 12, 13 |

## Results (from testing)
- Followed the line successfully in 9 out of 10 runs; small drift on sharp curves.
- Detected the correct 4-bit pattern whenever the clock-data timing was stable.
- About 100 ms between pattern detection and stopping.
- Around 45 minutes of running time on a 7.4 V 2000 mAh battery.

## Limitations
- No obstacle detection. An ultrasonic sensor and an LCD were planned but left out because the Arduino Uno ran out of I/O pins.
- Works best on a high-contrast track in moderate lighting.

## Future improvements
- Use an Arduino Mega or an I/O expander to add the ultrasonic sensor and LCD.
- Add Bluetooth or Wi-Fi for remote control.

## Code
The Arduino sketch is in `keylinebot.ino`. It uses the Keypad library (install it from the Arduino IDE Library Manager).

## Team
R. Abishek, N S Abhishek Vasanth Surya, Manikandan Madhan, Abijith T
