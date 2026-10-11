/*
  KeyLineBot - reconstructed sketch

  NOTE: This sketch was reconstructed from the project report (pin table,
  algorithm and the three code snippets). It is NOT the original file.
  Parts taken from the report: pins, line-following logic, sliding 4-bit
  comparator, forward() with PWM 132 (left) / 180 (right), 300 ms move / pause.
  Parts assumed: keypad entry method, leftTurn(), rightTurn(), stopBot().
  Test on the robot and adjust before relying on it.
*/

#include <Keypad.h>

// Motor driver (L293D)
#define ENA 5   // left motor speed
#define ENB 6   // right motor speed
#define IN1 2
#define IN2 3
#define IN3 4
#define IN4 7

// IR sensors (1 = white surface, 0 = black line)
#define LEFT_IR  8
#define RIGHT_IR 9
#define CLOCK_IR 10   // left side IR (clock)
#define DATA_IR  11   // right side IR (data)

// 4x4 keypad
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {A0, A1, A2, A3};
byte colPins[COLS] = {A4, A5, 12, 13};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const int LEFT_SPEED  = 132;
const int RIGHT_SPEED = 180;

int targetNum = 0;
String bitStream = "";
int prevClock = 0;
bool stopped = false;

int readTargetFromKeypad();
void forward();
void leftTurn();
void rightTurn();
void stopBot();

void setup() {
  Serial.begin(9600);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(LEFT_IR, INPUT);
  pinMode(RIGHT_IR, INPUT);
  pinMode(CLOCK_IR, INPUT);
  pinMode(DATA_IR, INPUT);

  stopBot();

  targetNum = readTargetFromKeypad();
  Serial.print("Target set to: ");
  Serial.println(targetNum);

  prevClock = digitalRead(CLOCK_IR);
}

void loop() {
  if (stopped) {
    stopBot();
    return;
  }

  // ---- Pattern detection (clock + data) ----
  int clockNow = digitalRead(CLOCK_IR);
  int data = digitalRead(DATA_IR);

  if (clockNow != prevClock) {              // on clock change
    prevClock = clockNow;
    bitStream += String(data);
    if (bitStream.length() > 20)
      bitStream.remove(0, bitStream.length() - 20);

    if (bitStream.length() >= 4) {          // need at least 4 bits to compare
      String last4 = bitStream.substring(bitStream.length() - 4);
      int value = strtol(last4.c_str(), NULL, 2);
      if (value == targetNum) {
        Serial.println("MATCH FOUND - stopping robot!");
        stopBot();
        stopped = true;
        return;
      }
    }
  }

  // ---- Line following ----
  int leftIR = digitalRead(LEFT_IR);
  int rightIR = digitalRead(RIGHT_IR);

  if (leftIR == 1 && rightIR == 1) forward();
  else if (leftIR == 0 && rightIR == 1) leftTurn();
  else if (leftIR == 1 && rightIR == 0) rightTurn();
  else stopBot();
}

// Type a number from 0 to 15 and press # to confirm. Press * to clear.
int readTargetFromKeypad() {
  while (true) {
    Serial.println("Enter target number (0-15), then press #");
    String entry = "";

    while (true) {
      char key = keypad.waitForKey();
      if (key == '#') break;
      if (key == '*') {
        entry = "";
        Serial.println("Cleared");
        continue;
      }
      if (key >= '0' && key <= '9' && entry.length() < 2) {
        entry += key;
        Serial.println(entry);
      }
    }

    if (entry.length() > 0) {
      int value = entry.toInt();
      if (value >= 0 && value <= 15) return value;
    }
    Serial.println("Invalid. Enter a number from 0 to 15.");
  }
}

void forward() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, LEFT_SPEED);
  analogWrite(ENB, RIGHT_SPEED);
  delay(300);   // move 300 ms
  stopBot();
  delay(300);   // pause 300 ms
}

// Assumed: left motor off, right motor forward
void leftTurn() {
  digitalWrite(IN1, LOW);  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, RIGHT_SPEED);
  delay(300);
  stopBot();
  delay(300);
}

// Assumed: right motor off, left motor forward
void rightTurn() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);  digitalWrite(IN4, LOW);
  analogWrite(ENA, LEFT_SPEED);
  analogWrite(ENB, 0);
  delay(300);
  stopBot();
  delay(300);
}

void stopBot() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
