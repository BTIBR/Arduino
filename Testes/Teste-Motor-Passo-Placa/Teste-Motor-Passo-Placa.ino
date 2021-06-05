#define enablePin 29
#define mode0Pin  28
#define mode1Pin  27
#define mode2Pin  26
#define resetPin  7
#define sleepPin  25
#define stepPin   24
#define dirPin    23
#define faultPin  22

unsigned long lastMicros;
String txtString;

void setup() {
  Serial.begin(9600);

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
}

void loop() {

  if (Serial.available() > 0) {
    txtString = Serial.readStringUntil('\n');             // Lê os bytes até '\n' e transforma em String
    Serial.flush();                                       // Limpa a Porta Serial
  }

  if (txtString == "M") {
    digitalWrite(dirPin, LOW);
    microstep(200);
  }

  if (txtString == "F") {
    digitalWrite(dirPin,  LOW);
    fullstep(200);
  }

  if (txtString == "K")   freio();

  if (txtString == "D")   desligado();

/*
  // Teste com CI 555

  digitalWrite(enablePin, LOW);
  digitalWrite(mode0Pin,  HIGH);
  digitalWrite(mode1Pin,  HIGH);
  digitalWrite(mode2Pin,  HIGH);
  digitalWrite(resetPin,  HIGH);
  digitalWrite(sleepPin,  HIGH);
  digitalWrite(dirPin,    HIGH);*/

  
}
