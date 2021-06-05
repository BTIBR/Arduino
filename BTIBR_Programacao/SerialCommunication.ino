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
