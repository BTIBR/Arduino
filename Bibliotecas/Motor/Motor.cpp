#include "Motor.h"

Motor::Motor(int pEnable, int pMode0, int pMode1, int pMode2, int pReset, int pSleep, int pStep, int pDir, int pFault) {
    PinEnable = pEnable;
    PinMode0  = pMode0;
    PinMode1  = pMode1;
    PinMode2  = pMode2;
    PinReset  = pReset;
    PinSleep  = pSleep;
    PinStep   = pStep;
    PinDir    = pStep;
    PinFault  = pFault;
}

void Motor::setMode(int Mode) {                                   // Seleção do modo de operação do Motor
    if (Mode == 0) {                                              // Full Step
      digitalWrite(PinMode0, LOW);
      digitalWrite(PinMode1, LOW);
      digitalWrite(PinMode2, LOW);
    }
    else if (Mode == 1) {                                         // Half Step
      digitalWrite(PinMode0, HIGH);
      digitalWrite(PinMode1, LOW);
      digitalWrite(PinMode2, LOW);
    }
    else if (Mode == 2) {                                         // 1/4 Microstepping
      digitalWrite(PinMode0, LOW);
      digitalWrite(PinMode1, HIGH);
      digitalWrite(PinMode2, LOW);
    }
    else if (Mode == 3) {                                         // 1/8 Microstepping
      digitalWrite(PinMode0, HIGH);
      digitalWrite(PinMode1, HIGH);
      digitalWrite(PinMode2, LOW);
    }
    else if (Mode == 4) {                                         // 1/16 Microstepping
      digitalWrite(PinMode0, LOW);
      digitalWrite(PinMode1, LOW);
      digitalWrite(PinMode2, HIGH);
    }
    else if (Mode == 5) {                                         // 1/32 Microstepping
      digitalWrite(PinMode0, HIGH);
      digitalWrite(PinMode1, HIGH);
      digitalWrite(PinMode2, HIGH);
    }
}

void Motor::setPins() {
  pinMode(PinEnable, OUTPUT);
  pinMode(PinMode0 , OUTPUT);
  pinMode(PinMode1 , OUTPUT);
  pinMode(PinMode2 , OUTPUT);
  pinMode(PinReset , OUTPUT);
  pinMode(PinSleep , OUTPUT);
  pinMode(PinStep  , OUTPUT);
  pinMode(PinDir   , OUTPUT);
  pinMode(PinFault , INPUT);
}