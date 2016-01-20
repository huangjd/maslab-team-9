#include <vector>

#include "Arduino.h"

#include "Config.h"
#include "Common.h"
#include "TransmissionLayer.h"
#include "ApplicationLayer.h"

void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, 1);
  delay(500);
  digitalWrite(13, 0);
  pinMode(13, INPUT);

  pinMode(28, OUTPUT);
  for (int i = 0; i < 256; i++) {
    analogWrite(28, i);
    delay(10);
  }
  pinMode(28, INPUT);

  analogReadResolution(ADC_RESOLUTION);
  analogWriteResolution(PWM_RESOLUTION);
  analogWriteFrequency(FTM0PIN, PWM0_FREQUENCY);
  analogWriteFrequency(FTM1PIN, PWM1_FREQUENCY);
  analogWriteFrequency(FTM2PIN, PWM2_FREQUENCY);

  Serial.setTimeout(1);
  Serial.flush();
  while (Serial.read() != -1) { // Clear serial buffer
  }
}

void loop() {
  getCommandBlocked();
  if (parseCommand()) {
    ackCommand();
  }
}

void cpuid() {
  uint32_t i = 0x411FC271;
  Serial.write((uint8_t*)&i, 4);
}


int main() {
  setup();
  while(1) {
    loop();
  }
}
