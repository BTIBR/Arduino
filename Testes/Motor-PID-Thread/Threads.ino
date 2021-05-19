void debugSerial() {
  Serial.print(M1.Setpoint);
  Serial.print(" ");
  Serial.print(posicao);
  Serial.print(" ");
  Serial.print(saida);
  //Serial.print(" ");
  //Serial.println(tempo, 3);
}

void executePID() {
  posicao = map(analogRead(pSENSOR), 0, 1023, 0, 100);
  saida   = M1.compute(posicao);

  if (saida > 0) {
    tempo = mapDouble(saida, 0.0, 100.0, tempoMax, tempoMin);
    moveMotor.setInterval(tempo);
    moveMotor.enabled = true;
  }
  else {
    moveMotor.enabled = false;
  }
}

void microstep() {
  digitalWrite(stepPin, !digitalRead(stepPin));
}

void piscaLed1() {
  digitalWrite(l1pin, !digitalRead(l1pin));
}

void piscaLed2() {
  digitalWrite(l2pin, !digitalRead(l2pin));
}
