#include "Commands.h"
#include "WProgram.h"
#include "Common.h"

#include <cstring>

Command::~Command() {
}

void Command::respond(TXBuffer *txbuf) {
}


bool Blink::process(const char *rxbuf) {
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


bool Echo::process(const char *rxbuf) {
  strncpy(buf, rxbuf, RX_MAX - 1);
  return true;
}

void Echo::respond(TXBuffer *txbuf) {
  strcpy(txbuf->buffer, buf);
  txbuf->send();
}


bool AnalogRead::process(const char *rxbuf) {
  int argn = sscanf(rxbuf, "%d", &pin);
  return argn && verifyPin(pin, ANALOG);
}

void AnalogRead::respond(TXBuffer *txbuf) {
  pinMode(pin, INPUT);
  int val = analogRead(pin);
  sprintf(txbuf->buffer, "%d", val);
  txbuf->send();
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
