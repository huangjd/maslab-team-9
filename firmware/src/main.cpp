#include <vector>

#include "Arduino.h"

#include "Commands.h"
#include "Config.h"
#include "MessageBuffer.h"

#define FIRMWARE_DEBUG 1

static void dit() {
#ifdef FIRMWARE_DEBUG
  pinMode(13, OUTPUT);
  digitalWrite(13, 1);
  delay(100);
  digitalWrite(13, 0);
  delay(100);
#endif
}

static void dat() {
#ifdef FIRMWARE_DEBUG
  pinMode(13, OUTPUT);
  digitalWrite(13, 1);
  delay(500);
  digitalWrite(13, 0);
  delay(100);
#endif
}

static void gap() {
#ifdef FIRMWARE_DEBUG
  delay(200);
#endif
}

static void debugPrint(const char* c) {
#ifdef FIRMWARE_DEBUG
  Serial.write(c, strlen(c));
  Serial.send_now();
#endif
}


void setup() {
  dat();

  //analogReadResolution(ADC_RESOLUTION);
  //analogWriteResolution(PWM_RESOLUTION);
  analogWriteFrequency(FTM0PIN, PWM0_FREQUENCY);
  analogWriteFrequency(FTM1PIN, PWM1_FREQUENCY);
  analogWriteFrequency(FTM2PIN, PWM2_FREQUENCY);

  Serial.setTimeout(0);
  Serial.flush();
  while (Serial.read() != -1) { // Clear serial buffer
  }

  debugPrint("Module Init OK\n");
}

void loop() {
  int c;
  if ((c = Serial.read()) == '\x01') {
    size_t size = Serial.readBytesUntil('\0', rxbuf, RX_MAX - 1);
    if (size) {
      rxbuf[size] = 0;
      if (!MessageBuffer::dispatch(rxbuf[0])) {
        dat();dat();gap();
        debugPrint("Bad command\n");
      }
    } else {
      dit();dit();gap();
      debugPrint("Empty message received\n");
    }
  }  else if (c != -1) {
    dit();gap();
    char msg[] = "Stray character   received\n";
    msg[16] = c;
    debugPrint(msg);
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
  return 0;
}

