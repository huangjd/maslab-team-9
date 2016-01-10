#include <vector>

#include "Arduino.h"

#include "Config.h"
#include "Common.h"
#include "TransmissionLayer.h"

void setup() {
  analogReadResolution(ADC_RESOLUTION);
  analogWriteResolution(PWM_RESOLUTION);
  analogWriteFrequency(FTM0PIN, PWM0_FREQUENCY);
  analogWriteFrequency(FTM1PIN, PWM1_FREQUENCY);
  analogWriteFrequency(FTM2PIN, PWM2_FREQUENCY);

  Serial.flush();
  while (Serial.read() != -1) { // Clear serial buffer
  }
}

void loop() {
  getCommandBlocked();
  // processCommand();
}

int main() {
  setup();
  while(1) {
    loop();
  }
}
