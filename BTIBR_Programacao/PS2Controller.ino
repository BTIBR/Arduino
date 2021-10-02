void SetupPS2() {
  // Setup dos pinos do controle, já checando erros
  PS2_Error = PS2.config_gamepad(pin_PS2_CLK, pin_PS2_CMD, pin_PS2_ATT, pin_PS2_DAT, PS2_PRESSURE, PS2_RUMBLE);
}

void ReadPS2() {
  PS2.read_gamepad(false, PS2_Vibrate);    // Ler controle
  if (PS2.NewButtonState()) {
    if (PS2.Button(PSB_RED))     Serial.println("Circulo");
    if (PS2.Button(PSB_GREEN))   Serial.println("Triangulo");
    if (PS2.Button(PSB_BLUE))    Serial.println("X");
    if (PS2.Button(PSB_START))   Serial.println("Start");
    if (PS2.Button(PSB_SELECT))  Serial.println("Select");
  }
  if (PS2.Button(PSB_L1))        Serial.println("L1");
  if (PS2.Button(PSB_R1))        Serial.println("R1");
  if (PS2.Button(PSB_PAD_UP))    Serial.println("UP");
  if (PS2.Button(PSB_PAD_RIGHT)) Serial.println("RIGHT");
  if (PS2.Button(PSB_PAD_LEFT))  Serial.println("LEFT");
  if (PS2.Button(PSB_PAD_DOWN))  Serial.println("DOWN");
}
