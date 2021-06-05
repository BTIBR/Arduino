void digiPot() {



  while (Serial.available() > 0) {                      // Aguarda dados na comunicação
    inChar = Serial.read();                             // Lê 1 caracter
    if (inChar != '\n') cmd += (char)inChar;            // Concatena na String se for diferente de \n
    if (inChar == '\n') {                               // Se for igual a \n
      pot.set(cmd.toInt());
      Serial.println(cmd); 
      cmd = "";
    }
  }
}



void testePCB() {
  M1.jog(1);
}
