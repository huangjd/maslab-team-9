#include "Terminate.h"
#include "WProgram.h"

void release() {

}

void drop() {

}

void endgame() {
  release();
  powerdown();
}

void powerdown() {
  for (int i = 0; i < 34; i++) {
    digitalWrite(i, LOW);
  }
  analogWrite(A10, 0);
  analogWrite(A11, 0);
  analogWrite(A12, 0);
  analogWrite(A13, 0);
  analogWrite(A14, 0);
}


void terminate() {
  powerdown();
  halt();
}
