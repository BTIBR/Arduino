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

  /*Serial.println(tempoM);

    digitalWrite(stepPin, HIGH);
    delayMicroseconds(tempoM);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(tempoM);*/

  if ((micros() - lastMicros) >= tempoM) {
    lastMicros = micros();
    digitalWrite(stepPin, !digitalRead(stepPin));
  }
}

void fullstep(int tempoF) {
  digitalWrite(mode0Pin,  LOW);
  digitalWrite(mode1Pin,  LOW);
  digitalWrite(mode2Pin,  LOW);

  digitalWrite(stepPin, HIGH);
  delayMicroseconds(tempoF);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(tempoF);
}
