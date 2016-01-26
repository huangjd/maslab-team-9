#include "Commands.h"
#include "MessageBuffer.h"
#include "WProgram.h"
#include "Common.h"

#include <cstring>

#include "SPI.h"

bool (*commandsRegister[64])() = {
    badcmd, badcmd, badcmd, badcmd, badcmd, badcmd, badcmd, badcmd, // @ABCDEFG
    badcmd, badcmd, badcmd, badcmd, badcmd, badcmd, badcmd, badcmd, // HIJKLMNO
    badcmd, badcmd, badcmd, badcmd, badcmd, badcmd, badcmd, badcmd, // PQRSTUVW
    badcmd, badcmd, badcmd, badcmd, badcmd, badcmd, badcmd, badcmd, // XYZ[\]^_
    badcmd, badcmd, blink,  badcmd, badcmd, echo ,  badcmd, badcmd, // `abcdefg
    badcmd, badcmd, badcmd, badcmd, badcmd, badcmd, badcmd, badcmd, // hijklmno
    badcmd, badcmd, badcmd, badcmd, badcmd, badcmd, badcmd, badcmd, // pqrstuvw
    badcmd, badcmd, badcmd, badcmd, badcmd, badcmd, badcmd, badcmd, // xyz{|}~
};

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
  strncpy(txbuf->buffer, rxbuf, RX_MAX - 1);
  txbuf->send();
}

static bool readAnalog() {
  int argn = sscanf(rxbuf, "%d", &pin);
  if (verifyPin(pin, ANALOG)) {
    pinMode(pin, INPUT);
    int val = analogRead(pin);
    sprintf(txbuf->buffer, "%d", val);
    txbuf->send();
    return true;
  }
  return false;
}


bool AnalogWrite::process(const char *rxbuf) {
  int pin, val;
  int argn = sscanf(rxbuf, "%d %d", &pin, &val);
  if (argn == 2 && verifyPin(pin, PWM)) {
    pinMode(pin, OUTPUT);
    analogWrite(pin, val);
    return true;
  }
  return false;
}


bool DigitalRead::process(const char *rxbuf) {
  int argn = sscanf(rxbuf, "%d", &pin);
  return argn && verifyPin(pin, DIGITAL);
}

void DigitalRead::respond(TXBuffer *txbuf) {
  pinMode(pin, INPUT);
  int val = digitalRead(pin);
  sprintf(txbuf->buffer, "%d", val);
  txbuf->send();
}


bool DigitalWrite::process(const char *rxbuf) {
  int pin, val;
  int argn = sscanf(rxbuf, "%d %d", &pin, &val);
  if (argn == 2 && verifyPin(pin, DIGITAL) && (val == 0 || val == 1)) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, val);
    return true;
  }
  return false;
}

bool GyroCalibrate::process(const char *rxbuf) {
  pinMode(GYRO_CS, OUTPUT);
  digitalWrite(GYRO_CS, HIGH);
  SPI.begin();


  return true;
}









































