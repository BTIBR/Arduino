void freio() {
  digitalWrite(enablePin, LOW);
}

void desligado() {
  digitalWrite(enablePin, HIGH);
}

/*void microstep(int tempoM) {
  digitalWrite(mode0Pin,  HIGH);
  digitalWrite(mode1Pin,  HIGH);
  digitalWrite(mode2Pin,  HIGH);

  if ((micros() - lastMicros) >= tempoM) {
    digitalWrite(stepPin, !digitalRead(stepPin));
    lastMicros = micros();
  }
}*/

void fullstep(int tempoF) {
  digitalWrite(mode0Pin,  LOW);
  digitalWrite(mode1Pin,  LOW);
  digitalWrite(mode2Pin,  LOW);

  digitalWrite(stepPin, HIGH);
  delayMicroseconds(tempoF);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(tempoF);
}



double mapDouble (double x, double in_min, double in_max, double out_min, double out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
