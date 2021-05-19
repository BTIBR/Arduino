#define dirPin 8
#define stepPin 9
#define stepsPerRevolution 200

int tempo = 1000;

void setup() {
  // Declare pins as output:
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
}
void loop() {

  digitalWrite(dirPin, HIGH);

  digitalWrite(stepPin, HIGH);
  delayMicroseconds(tempo);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(tempo);
}
