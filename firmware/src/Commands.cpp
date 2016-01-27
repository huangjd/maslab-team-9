#include "Commands.h"
#include "MessageBuffer.h"
#include "WProgram.h"
#include "Common.h"
#include "Operation.h"

#include <cstring>

#include "SPI.h"

static bool badcmd() {
  return false;
}

static bool blink() {
  int val = 1;
  sscanf(rxbuf, "%d", &val);
  pinMode(13, OUTPUT);
  for (int i = 0; i < val; i++) {
    digitalWrite(13, HIGH);
    delay(500);
    digitalWrite(13, LOW);
    delay(500);
  }
  return true;
}

static bool echo() {
  strncpy(txbuf.buffer, rxbuf + 1, RX_MAX - 1);

  return true;
}

static bool readAnalog() {
  int pin;
  if (sscanf(rxbuf, "%d", &pin) && verifyPin(pin, ANALOG)) {
    pinMode(pin, INPUT);
    int val = analogRead(pin);
    sprintf(txbuf.buffer, "%d", val);

    return true;
  } else {
    return false;
  }
}


static bool writeAnalog() {
  int pin, val;
  int argn = sscanf(rxbuf, "%d %d", &pin, &val);
  if (argn == 2 && verifyPin(pin, PWM)) {
    pinMode(pin, OUTPUT);
    analogWrite(pin, val);
    return true;
  }
  return false;
}

static bool readDigital() {
  int pin;
  if (sscanf(rxbuf, "%d", &pin) == 1 && verifyPin(pin, DIGITAL)) {
    pinMode(pin, INPUT);
    sprintf(txbuf.buffer, "%d", digitalRead(pin));

    return true;
  } else {
    return false;
  }
}

static bool writeDigital() {
  int pin, val;
  if (sscanf(rxbuf, "%d %d", &pin, &val) == 2 &&
      verifyPin(pin, DIGITAL) && (val == 0 || val == 1)) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, val);
    return true;
  } else {
    return false;
  }
}

static bool halt() {
  analogWrite(WHEEL_SPEED_L, 0);
  analogWrite(WHEEL_SPEED_R, 0);
  digitalWrite(STEPPER_R_EN, 0);
  digitalWrite(STEPPER_R_CLK, 0);
  digitalWrite(STEPPER_L_EN, 0);
  digitalWrite(STEPPER_L_CLK, 0);
  digitalWrite(CONTAINER_L_1, 0);
  digitalWrite(CONTAINER_L_2, 0);
  digitalWrite(CONTAINER_R_1, 0);
  digitalWrite(CONTAINER_R_2, 0);
  return true;
}

static bool moveForward() {
  float val;
  if (sscanf(rxbuf, "%f", &val)) {
    if (val > 0) {
      digitalWrite(WHEEL_DIR_L, 1);
      digitalWrite(WHEEL_DIR_R, 0);
    } else {
      digitalWrite(WHEEL_DIR_L, 0);
      digitalWrite(WHEEL_DIR_R, 1);
    }
    analogWrite(WHEEL_SPEED_L, 255);
    analogWrite(WHEEL_SPEED_R, 255);
    delay(fabs(val) * FORWARD_CONSTANT);
    analogWrite(WHEEL_SPEED_L, 0);
    analogWrite(WHEEL_SPEED_R, 0);
    return true;
  }
  return false;
}

static bool turn() {
  float val;
  if (sscanf(rxbuf, "%f", &val)) {
    if (val > 0) {
      digitalWrite(WHEEL_DIR_L, 0);
      digitalWrite(WHEEL_DIR_R, 0);
    } else {
      digitalWrite(WHEEL_DIR_L, 1);
      digitalWrite(WHEEL_DIR_R, 1);
    }
    analogWrite(WHEEL_SPEED_L, 255);
    analogWrite(WHEEL_SPEED_R, 255);
    delay(fabs(val) * TURN_CONSTANT);
    analogWrite(WHEEL_SPEED_L, 0);
    analogWrite(WHEEL_SPEED_R, 0);
    return true;
  }
  return false;
}

static bool step() {
  unsigned int mode;
  if (sscanf(rxbuf, "%u", &mode) && mode < 4) {
    stepperOperation(mode, 1);
    return true;
  }
  return false;
}

static bool clamp() {
  unsigned int side;
  unsigned int pwm;

  if (sscanf(rxbuf, "%u %u", &side, &pwm) == 2 && side < 2) {
    if (side & RIGHT) {
      analogWrite(CLAMP_R, pwm);
    } else {
      analogWrite(CLAMP_L, pwm);
    }
    return true;
  }
  return false;
}

static bool door() {
  unsigned int mode;
  int amount;

  if (sscanf(rxbuf, "%u %d", &mode, &amount) == 2 && mode < 4) {
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
    delay(DOOR_MOTOR_SCALE * amount);
    digitalWrite(pos, 0);
    return true;
  }
  return false;
}

static bool readIR() {
  unsigned int order;
  if (sscanf(rxbuf, "%u", &order) && order < 4) {
    switch (order) {
    case 0:
      sprintf(txbuf.buffer, "%d %d", analogRead(SHORT_IR_L), digitalRead(ULTRA_SHORT_IR_L));
      break;
    case 1:
      sprintf(txbuf.buffer, "%d %d", analogRead(SHORT_IR_B), digitalRead(ULTRA_SHORT_IR_B));
      break;
    case 2:
      sprintf(txbuf.buffer, "%d %d", analogRead(SHORT_IR_R), digitalRead(ULTRA_SHORT_IR_R));
      break;
    case 3:
      sprintf(txbuf.buffer, "%d %d", analogRead(SHORT_IR_F), digitalRead(ULTRA_SHORT_IR_F));
      break;
    }

  }
  return false;
}

static bool pickup() {
  unsigned int side;
  if (sscanf(rxbuf, "%u", &side) && (side < 2)) {
    stepperOperation(side | DOWN);
    clampOperation(side | OPEN);
    stepperOperation(side | DOWN);
    clampOperation(side | CLOSE);
    stepperOperation(side | UP | STEPPER_TWICE);
    return true;
  }
  return false;
}

static bool release() {
  unsigned int mode;
  if (sscanf(rxbuf, "%u", &mode) && mode < 4) {
    bool platform = mode & 2;
    mode = mode & 1;
    if (platform) {
      clampOperation(mode | OPEN);
      doorOperation(mode | OPEN);
      emergencyBackUp();
    } else {
      stepperOperation(mode | DOWN | STEPPER_TWICE);
      clampOperation(mode | OPEN);
      doorOperation(mode | OPEN);
      emergencyBackUp();
    }
    return true;
  }
  return false;
}

static bool readIRDist() {
  return false;
}

bool (*commandsRegister[64])() = {
    badcmd, writeAnalog, badcmd, badcmd, writeDigital, badcmd, badcmd, badcmd, // @ABCDEFG
    halt, readIRDist, badcmd, badcmd, badcmd, moveForward, badcmd, badcmd, // HIJKLMNO
    pickup, badcmd, release, badcmd, turn, badcmd, badcmd, badcmd, // PQRSTUVW
    badcmd, badcmd, badcmd, badcmd, badcmd, badcmd, badcmd, badcmd, // XYZ[\]^_
    badcmd, readAnalog, blink, clamp, readDigital, echo,  badcmd, badcmd, // `abcdefg
    badcmd, readIR, badcmd, badcmd, badcmd, badcmd, badcmd, badcmd, // hijklmno
    badcmd, badcmd, badcmd, step, badcmd, badcmd, badcmd, badcmd, // pqrstuvw
    badcmd, badcmd, door, badcmd, badcmd, badcmd, badcmd, badcmd, // xyz{|}~
};


