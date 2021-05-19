#define enablePin 2
#define mode0Pin  3
#define mode1Pin  4
#define mode2Pin  5
#define resetPin  6
#define sleepPin  7
#define stepPin   8
#define dirPin    9
#define faultPin  10

#include <MotorPID.h>
#define pSENSOR     A0
double posicao = 0.0;
double saida   = 0.0;
MotorPID M1(dirPin);

int tempo = 50;
double tempoMin = 0;
double tempoMax = 100;
unsigned long lastMicros;

void setup() {
  //Serial.begin(9600);

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

  pinMode(pSENSOR, INPUT);

  M1.Kp       = 1.0;
  M1.Ki       = 0.0;
  M1.Kd       = 0.0;
  M1.Setpoint = 45.0;
  M1.DeadBand = 1.0;
  M1.Direct   = true;
}

void loop() {

  /*posicao = map(analogRead(pSENSOR), 0, 1023, 0, 100);
  saida   = M1.compute(posicao);

  if (saida > 0) {
    tempo = map(saida, 0, 100, tempoMax, tempoMin);*/
    microstep(tempo);
  /*}
  else {
    digitalWrite(enablePin, LOW);
  }*/
  /*
      Serial.print(M1.Setpoint);
      Serial.print(" ");
      Serial.print(posicao);
      Serial.print(" ");
      Serial.print(saida);
      Serial.print(" ");
      Serial.println(tempo);*/
}
