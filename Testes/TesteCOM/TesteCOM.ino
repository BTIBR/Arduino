char in[10];                              // Valores de entrada da porta de comunicação serial
String inputString = "";                  // String que armazena os dados vindos da serial
bool stringComplete = false;              // Flag de finalização de uma palavra
String posicaoGarra = "";                 // Parte da String de comunicação que significa a posição da Garra em graus

void setup() {
  Serial.begin(9600);

  pinMode(39, OUTPUT);
}

void loop() {

  delay(10);
}

void serialEvent() {
  while (Serial.available()) {

    digitalWrite(39, HIGH);

    char inChar = (char)Serial.read();    // Lê o novo byte
    inputString += inChar;                // Adiciona o novo byte na String completa
    if (inChar == 'B') {                  // Se o novo byte for 'B'
      stringComplete = true;              // Finaliza aquela palavra
      delay(500);
      digitalWrite(39, LOW);
    }
  }
}
