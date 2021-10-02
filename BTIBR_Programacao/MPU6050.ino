void SetupMPU6050() {
  Wire.begin();                          // Inicializa a bilioteca Wire
  Wire.beginTransmission(MPU);           // Inicia a transmissão para o endereço do MPU
  Wire.write(0x6B);                      // Escreve o comando de inicialização
  Wire.write(0);                         // Limpa o canal de escrita
  Wire.endTransmission(true);            // Finaliza a transmissão
}

void ReadMPU6050() {
  // Processo de Comunicação e Transmissão dos dados
  Wire.beginTransmission(MPU);           // Inicia a transmissão para o endereço do MPU
  Wire.write(0x3B);                      // Define o registrador 0x3B (ACCEL_XOUT_H) como primeiro da lista de leitura
  Wire.endTransmission(false);           // Mantém a transmissão ativa
  Wire.requestFrom(MPU, 14, true);       // Solicita os dados do sensor

  // Armazena o valor dos sensores nas variaveis correspondentes
  aX  = Wire.read() << 8 | Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  aY  = Wire.read() << 8 | Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  aZ  = Wire.read() << 8 | Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp = Wire.read() << 8 | Wire.read();  // 0x41 (TEMP_OUT_H)   & 0x42 (TEMP_OUT_L)
  gX  = Wire.read() << 8 | Wire.read();  // 0x43 (GYRO_XOUT_H)  & 0x44 (GYRO_XOUT_L)
  gY  = Wire.read() << 8 | Wire.read();  // 0x45 (GYRO_YOUT_H)  & 0x46 (GYRO_YOUT_L)
  gZ  = Wire.read() << 8 | Wire.read();  // 0x47 (GYRO_ZOUT_H)  & 0x48 (GYRO_ZOUT_L)

  Serial.println(aX);
  Serial.println(aY);
  Serial.println(aZ);
  Serial.println(Tmp);
  Serial.println(gX);
  Serial.println(gY);
  Serial.println(gZ);
  Serial.println("**********************************************");
}
