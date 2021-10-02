void SetupSerial() {
  Serial.begin(9600);                           // Inciando a porta de comunicação serial
  cmd.reserve(20);                              // Reservando até 20 bytes para a palavra de comunicação
  Serial.println("Bancada de Teste de Pré-Carga em Fusos de Esfera Integrada com Braço Robótico\n");
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();    // Lê o novo byte
    cmd += inChar;                        // Adiciona o novo byte na String completa
    if (inChar == '\n') {                 // Se o novo byte for 'B'
      stringComplete = true;              // Finaliza aquela palavra
      Serial.println(cmd);                // Escreve na tela o comando recebido
    }
  }
}
