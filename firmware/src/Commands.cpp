#include "Commands.h"
#include "MessageBuffer.h"
#include "WProgram.h"
#include "Common.h"

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
  txbuf.send();
  return true;
}

static bool readAnalog() {
  int pin;
  int argn = sscanf(rxbuf, "%d", &pin);
  if (!verifyPin(pin, ANALOG)) {
    return false;
  }
  pinMode(pin, INPUT);
  int val = analogRead(pin);
  sprintf(txbuf.buffer, "%d", val);
  txbuf.send();
  return true;
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
    txbuf.send();
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
  digitalWrite(WHEEL_SPEED_L, 0);
  digitalWrite(WHEEL_SPEED_R, 0);
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

/*
bool GyroCalibrate::process(const char *rxbuf) {
  pinMode(GYRO_CS, OUTPUT);
  digitalWrite(GYRO_CS, HIGH);
  SPI.begin();


  return true;
}*/

































bool (*commandsRegister[64])() = {
    badcmd, writeAnalog, badcmd, badcmd, writeDigital, badcmd, badcmd, badcmd, // @ABCDEFG
    halt, badcmd, badcmd, badcmd, badcmd, badcmd, badcmd, badcmd, // HIJKLMNO
    badcmd, badcmd, badcmd, badcmd, badcmd, badcmd, badcmd, badcmd, // PQRSTUVW
    badcmd, badcmd, badcmd, badcmd, badcmd, badcmd, badcmd, badcmd, // XYZ[\]^_
    badcmd, readAnalog, blink,  badcmd, readDigital, echo,  badcmd, badcmd, // `abcdefg
    badcmd, badcmd, badcmd, badcmd, badcmd, badcmd, badcmd, badcmd, // hijklmno
    badcmd, badcmd, badcmd, badcmd, badcmd, badcmd, badcmd, badcmd, // pqrstuvw
    badcmd, badcmd, badcmd, badcmd, badcmd, badcmd, badcmd, badcmd, // xyz{|}~
};








