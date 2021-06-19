#include <X9C103S.h>

#define pin_M1_CS   44
#define pin_M1_INC  45
#define pin_M1_UD   46

X9C103S POT;

// Comunicação Serial
String cmd = "";                          // String que armazena os dados vindos da serial
bool stringComplete = false;              // Flag de finalização de uma palavra

// Leitura Analógica
double soma = 0.0;

void setup() {
  POT.SetPins(pin_M1_CS, pin_M1_INC, pin_M1_UD);
  Serial.begin(9600);
  POT.Reset();
  pinMode(A1, INPUT);
}

/*

  99 = 1023
  0 = 0
  50 = 516



*/

void loop() {

  if (stringComplete) {
    int counts = cmd.toInt();
    POT.Set(counts);
    delay(500);

    for (int i = 0; i < 1000; i++) {
      soma += analogRead(A1);
    }
    double media = soma / 1000.0;
    double tensao = (media*5.0)/1023.0;
    soma = 0;
    
    Serial.print("Counts: ");
    Serial.println(counts);
    Serial.print("Resistência: ");
    Serial.println(counts * 101);
    Serial.print("Tensão: ");
    Serial.println(tensao);
    Serial.println();;

    cmd = "";                                         // Limpa a variável que armazena os comandos
    stringComplete = false;                           // Define como falso a flag de String Completa
  }
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();    // Lê o novo byte
    cmd += inChar;                        // Adiciona o novo byte na String completa
    if (inChar == '\n') {                 // Se o novo byte for 'B'
      stringComplete = true;              // Finaliza aquela palavra
      //Serial.println(cmd);                // Escreve na tela o comando recebido
    }
  }
}
