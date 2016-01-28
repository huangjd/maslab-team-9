#include <vector>

#include "Arduino.h"

#include "Commands.h"
#include "Config.h"
#include "MessageBuffer.h"
#include "Operation.h"
#include "Servo.h"

Servo leftServo, rightServo;

static void dit() {
#ifndef NDEBUG
  pinMode(13, OUTPUT);
  digitalWrite(13, 1);
  delay(100);
  digitalWrite(13, 0);
  delay(100);
#endif
}

static void dat() {
#ifndef NDEBUG
  pinMode(13, OUTPUT);
  digitalWrite(13, 1);
  delay(500);
  digitalWrite(13, 0);
  delay(100);
#endif
}

static void gap() {
#ifndef NDEBUG
  delay(200);
#endif
}

static void debugPrint(const char* c) {
  size_t len = strlen(c);
  char buf[len + 4];
  buf[0] = '\x01';
  buf[1] = len;
  strncpy(buf + 2, c, len);
  buf[len + 2] = '\n';
  buf[len + 3] = '\0';
  Serial.write(buf, len + 4);
  Serial.send_now();
}


void setup() {
  analogReadResolution(ADC_RESOLUTION);
  analogWriteResolution(PWM_RESOLUTION);
  analogWriteFrequency(FTM0PIN, PWM0_FREQUENCY);
  analogWriteFrequency(FTM1PIN, PWM1_FREQUENCY);
  analogWriteFrequency(FTM2PIN, PWM2_FREQUENCY);

  Serial.setTimeout(0);
  Serial.flush();
  while (Serial.read() != -1) { // Clear serial buffer
  }

  dat();

  for (int i : {WHEEL_DIR_L, WHEEL_DIR_R, WHEEL_SPEED_L, WHEEL_SPEED_R,
                STEPPER_L_EN, STEPPER_L_CLK, STEPPER_L_DIR,
                STEPPER_R_EN, STEPPER_R_CLK, STEPPER_R_DIR,
                CLAMP_L, CLAMP_R,
                CONTAINER_L_1, CONTAINER_L_2, CONTAINER_R_1, CONTAINER_R_2}) {
    pinMode(i, OUTPUT);
    digitalWrite(i, 0);
  }

  for (int i : {ULTRA_SHORT_IR_L, ULTRA_SHORT_IR_B, ULTRA_SHORT_IR_R, ULTRA_SHORT_IR_F,
                SHORT_IR_L, SHORT_IR_B, SHORT_IR_R, SHORT_IR_F,
                ENCODER_L_A, ENCODER_L_B, ENCODER_R_A, ENCODER_R_B}) {
    pinMode(i, INPUT);
  }

  leftServo.attach(CLAMP_L);
  rightServo.attach(CLAMP_R);
  leftServo.write(CLAMP_CLOSE);
  rightServo.write(CLAMP_CLOSE);

  stepperOperation(LEFT, DOWN, 10);
  stepperOperation(RIGHT, DOWN, 10);
  stepperOperation(LEFT, UP, STEPPER_STEP_1 + STEPPER_STEP_2);
  stepperOperation(RIGHT, UP, STEPPER_STEP_1 + STEPPER_STEP_2);

  //debugPrint("Module Init OK\n");
}

void loop() {
  int c;
  if ((c = Serial.read()) == '\x01') {
    size_t size = Serial.readBytesUntil('\0', rxbuf, RX_MAX - 1);
    if (size) {
      rxbuf[size] = 0;
      if (!MessageBuffer::dispatch()) {
        dit();dit();dit();gap();
        debugPrint("Bad command\n");
      } else {
        txbuf.send();
      }
    } else {
      dit();dit();gap();
      debugPrint("Empty message received\n");
    }
  } else if (c != -1) {
    dit();gap();
    char msg[] = "Stray character   received\n";
    msg[16] = c;
    debugPrint(msg);
  }
}

bool StackRed = false;

bool alive = true;

extern void halt();
int main() {
  setup();
  while(alive) {
    loop();
  }
  halt();
  delay(1000000);
  return 0;
}

