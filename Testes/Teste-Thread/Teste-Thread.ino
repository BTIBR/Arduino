#include "Thread.h"
#include "ThreadController.h"

#define l1pin 49
#define l2pin 47

void piscaLed1() {
  digitalWrite(l1pin, !digitalRead(l1pin));
}

void piscaLed2() {
  digitalWrite(l2pin, !digitalRead(l2pin));
}

ThreadController CPU;

Thread threadL1;
Thread threadL2;

void setup() {

  pinMode(l1pin, OUTPUT);
  pinMode(l2pin, OUTPUT);
  
  threadL1.setInterval(500000);
  threadL1.onRun(piscaLed1);

  threadL2.setInterval(300000);
  threadL2.onRun(piscaLed2);

  CPU.add(&threadL1);
  CPU.add(&threadL2);
}

void loop() {
  CPU.run();  
}
