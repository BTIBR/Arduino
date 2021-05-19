#include <MotorPID.h>

#define pSENSOR     A0
#define M1_DIR_PIR  1

double posicao = 0.0;
double saida   = 0.0;

MotorPID M1(M1_DIR_PIR);

void setup() {
  Serial.begin(9600);
  pinMode(pSENSOR, INPUT);
  pinMode(M1_DIR_PIR, OUTPUT);

  M1.Kp       = 1.0;
  M1.Ki       = 0.0;
  M1.Kd       = 0.0;
  M1.Setpoint = 45.0;
  M1.DeadBand = 1.0;
  M1.Direct   = false;
}

void loop() {
  
  posicao = map(analogRead(pSENSOR), 0, 1023, 0, 100);
  saida   = M1.compute(posicao);

  Serial.print(M1.Setpoint);
  Serial.print(" ");
  Serial.print(posicao);
  Serial.print(" ");
  Serial.println(saida);
}
