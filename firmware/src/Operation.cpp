#include "Operation.h"
#include "WProgram.h"
#include "Servo.h"

void clock(int pin, int hold, int d) {
  digitalWrite(pin, 1);
  if (hold) {
    delay(hold);
  }
  digitalWrite(pin, 0);
  if (d) {
    delay(d);
  }
}

extern Servo leftServo, rightServo;
void clampOperation(bool right, int angle) {
  if (right) {
    rightServo.write(angle);
  } else {
    leftServo.write(angle);
  }
}

void doorOperation(unsigned int mode) {
  unsigned int pos, neg;
  if (mode & RIGHT) {
    if (mode & CLOSE) {
      pos = CONTAINER_R_1;
      neg = CONTAINER_R_2;
    } else {
      pos = CONTAINER_R_2;
      neg = CONTAINER_R_1;
    }
  } else {
    if (mode & CLOSE) {
      pos = CONTAINER_L_1;
      neg = CONTAINER_L_2;
    } else {
      pos = CONTAINER_L_2;
      neg = CONTAINER_L_1;
    }
  }

  digitalWrite(pos, 1);
  digitalWrite(neg, 0);
  delay(DOOR_MOTOR_CONSATANT);
  digitalWrite(pos, 0);
}

void stepperOperation(bool right, bool down, int steps) {
  unsigned int clk = (right ? STEPPER_R_CLK : STEPPER_L_CLK);
  unsigned int en = (right ? STEPPER_R_EN : STEPPER_L_EN);
  unsigned int dir = (right ? STEPPER_R_DIR : STEPPER_L_DIR);

  pinMode(en, OUTPUT);
  pinMode(clk, OUTPUT);
  pinMode(dir, OUTPUT);

  digitalWrite(en, 1);

  digitalWrite(dir, !down);
  delay(STEPPER_DELAY * 2);

  for (int i = 0; i < steps; i++) {
    clock(clk, STEPPER_DELAY, STEPPER_DELAY);
  }

  digitalWrite(en, 0);
}

void emergencyBackUp() {
  digitalWrite(WHEEL_DIR_L, 0);
  digitalWrite(WHEEL_DIR_R, 1);
  analogWrite(WHEEL_SPEED_L, 255);
  analogWrite(WHEEL_SPEED_R, 255);
  delay(EMERGENCY_BACKUP_AMOUNT);
  analogWrite(WHEEL_SPEED_L, 0);
  analogWrite(WHEEL_SPEED_R, 0);
}
