void freio() {
  digitalWrite(enablePin, LOW);
}

void desligado() {
  digitalWrite(enablePin, HIGH);
}

void microstep(int tempoM) {
  digitalWrite(mode0Pin,  HIGH);
  digitalWrite(mode1Pin,  HIGH);
  digitalWrite(mode2Pin,  HIGH);

  digitalWrite(stepPin,  LOW);

  /*if ((micros() - lastMicros) >= tempoM) {
    digitalWrite(stepPin, !digitalRead(stepPin));
    lastMicros = micros();
  }*/
}

void fullstep(int tempoF) {
  digitalWrite(mode0Pin,  LOW);
  digitalWrite(mode1Pin,  LOW);
  digitalWrite(mode2Pin,  LOW);
  
  digitalWrite(stepPin, LOW);
  /*delayMicroseconds(tempoF);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(tempoF);*/
}
