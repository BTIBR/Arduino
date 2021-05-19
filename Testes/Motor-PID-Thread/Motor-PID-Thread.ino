#include <MotorPID.h>
#include "ThreadMicros.h"
#include "ThreadControllerMicros.h"

#define enablePin 2
#define mode0Pin  3
#define mode1Pin  4
#define mode2Pin  5
#define resetPin  6
#define sleepPin  7
#define stepPin   8
#define dirPin    9
#define faultPin  10

#define l1pin 49
#define l2pin 47

#define pSENSOR     A0
double posicao = 0.0;
double saida   = 0.0;
MotorPID M1(dirPin);

double tempoMin = 0;
double tempoMax = 200;
double tempo = 50;

ThreadController CPU;
Thread calculoPID;
Thread printSerial;
Thread moveMotor;
Thread threadL1;
Thread threadL2;

void setup() {
  Serial.begin(9600);

  calculoPID.setInterval(20000);
  calculoPID.onRun(executePID);

  printSerial.setInterval(200000);
  printSerial.onRun(debugSerial);

  moveMotor.setInterval(tempo);
  moveMotor.onRun(microstep);

  threadL1.setInterval(500000);
  threadL1.onRun(piscaLed1);

  threadL2.setInterval(300000);
  threadL2.onRun(piscaLed2);

  CPU.add(&calculoPID);
  CPU.add(&printSerial);
  CPU.add(&moveMotor);
  CPU.add(&threadL1);
  CPU.add(&threadL2);

  pinMode(enablePin, OUTPUT);
  pinMode(mode0Pin,  OUTPUT);
  pinMode(mode1Pin,  OUTPUT);
  pinMode(mode2Pin,  OUTPUT);
  pinMode(resetPin,  OUTPUT);
  pinMode(sleepPin,  OUTPUT);
  pinMode(stepPin,   OUTPUT);
  pinMode(dirPin,    OUTPUT);
  pinMode(faultPin,  INPUT);

  digitalWrite(enablePin, LOW);
  digitalWrite(resetPin,  HIGH);
  digitalWrite(sleepPin,  HIGH);

  digitalWrite(mode0Pin,  HIGH);          // uStep
  digitalWrite(mode1Pin,  HIGH);
  digitalWrite(mode2Pin,  HIGH);

  pinMode(pSENSOR, INPUT);

  M1.Kp       = 1.0;
  M1.Ki       = 0.0;
  M1.Kd       = 0.0;
  M1.Setpoint = 45.0;
  M1.DeadBand = 0.0;
  M1.Direct   = true;

  //printSerial.enabled = false;
}

void loop() {

  CPU.run();
}
