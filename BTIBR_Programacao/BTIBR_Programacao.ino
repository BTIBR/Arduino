/************************************************************************************************************************
                                                  SENAI "Roberto Mange"
                             Bruno Camargo - Lucas Melaré - Rafael Falcaro - Rubens Lima
                                                         3STMI
                                                         2021
************************************************************************************************************************/


#include <X9C103S.h>
DigiPot pot(45,46,44);    // INC, UD, CS
char inChar;
String cmd;



//***********************************************************************************************************************
// Inclusão das Bibliotecas e Header dos pinos da placa
#include <Motor.h>                        // Biblioteca customizada para os Motores de Passo
/*#include <Servo.h>                        // Biblioteca para motores Servo
#include <Wire.h>                         // Biblioteca para uso do Giroscópio MPU6050
#include <PS2X_lib.h>                     // Biblioteca do controle de PlayStation 2 sem fio*/
#include "PinoutConfig.h"                 // Header de configuração dos pinos da placa Arduino Mega

//***********************************************************************************************************************
// Criação dos Objetos
Motor M1(pin_M1_FAULT, pin_M1_DIR, pin_M1_SLEEP, pin_M1_RESET, pin_M1_MODE2, pin_M1_MODE1, pin_M1_ENABLE, pin_M1_MODE0, pin_M1_STEP);      // Motor 1 - Base (Rz)
/*Motor M2(0, 0, 0, 0, 0, 0, 0, 0, 0);      // Motor 2 - Ombro (Rx1)
Motor M3(0, 0, 0, 0, 0, 0, 0, 0, 0);      // Motor 3 - Antebraço (Rx2)
Servo M4;                                 // Motor 4 - Garra
PS2X  Controle;                           // Controle de PlayStation 2 sem fio*/

//***********************************************************************************************************************
// Declaração de Variáveis
/*
// Sensor Giroscópio
const int MPU = 0x68;                     // Endereco I2C do MPU6050
int aX, aY, aZ, Tmp, gX, gY, gZ;          // Variaveis para armazenar valores dos sensores (a=Aceleração | Tmp=Temperatura | g=Rotação)

// Controle PS2 sem fio
int  PS2_Error    = 0;                    // Tratamento de erro de inicialização da comunicação com o controle sem fio
byte PS2_Vibrate  = 0;                    // Velocidade da vibração do controle (0 a 255)
bool PS2_PRESSURE = true;                 // Ler pressões?
bool PS2_RUMBLE   = true;                 // Ler rumbles?

// Comunicação Serial
char in[10];                              // Valores de entrada da porta de comunicação serial
String inputString = "";                  // String que armazena os dados vindos da serial
bool stringComplete = false;              // Flag de finalização de uma palavra
String posicaoGarra = "";                 // Parte da String de comunicação que significa a posição da Garra em graus
*/
//***********************************************************************************************************************
// Setup do Sistema
void setup() {

  // Comunicação Serial
  Serial.begin(9600);                     // Inciando a porta de comunicação serial
  //Serial.println("start");

  //inputString.reserve(20);                // Reservando até 20 bytes para a palavra de comunicação

  // Motores
  M1.setPins();                           // Definindo as entradas e saídas do M1
  //M1.setMode("1/32");
  M1.setMode("FULL");

  
  /*M2.setPins();                           // Definindo as entradas e saídas do M2
  M3.setPins();                           // Definindo as entradas e saídas do M3
  M4.attach(9);                           // Definindo o pino de saída PWM do M4

  // Sensor Giroscópio e Controle PS2 sem fio
  SetupMPU6050();                         // Setup do sensor MPU6050
  SetupControle();                        // Setup do controle sem fio de PS2

  pinMode(39, OUTPUT);
  pinMode(41, OUTPUT);
  pinMode(43, OUTPUT);
  pinMode(45, OUTPUT);
  pinMode(47, OUTPUT);
  pinMode(49, OUTPUT);

  digitalWrite(39, HIGH);
  digitalWrite(41, HIGH);
  digitalWrite(43, HIGH);
  digitalWrite(45, HIGH);
  digitalWrite(47, HIGH);
  digitalWrite(49, HIGH);

  M4.write(0);

  delay(5000);

  digitalWrite(39, LOW);
  digitalWrite(41, LOW);
  digitalWrite(43, LOW);
  digitalWrite(45, LOW);
  digitalWrite(47, LOW);
  digitalWrite(49, LOW);*/
}

//***********************************************************************************************************************
// Rotina principal
void loop() {


  digiPot();
  testePCB();
  
/*
  if (stringComplete) {
    if (inputString[4] == '1') {
      digitalWrite(49, HIGH);
    }
    else if (inputString[4] == '0') {
      digitalWrite(49, LOW);
    }

    if (inputString[5] == '1') {
      digitalWrite(47, HIGH);
    }
    else if (inputString[5] == '0') {
      digitalWrite(47, LOW);
    }

    if (inputString[6] == '1') {
      digitalWrite(45, HIGH);
    }
    else if (inputString[6] == '0') {
      digitalWrite(45, LOW);
    }

    if (inputString[7] == '1') {
      digitalWrite(43, HIGH);
    }
    else if (inputString[7] == '0') {
      digitalWrite(43, LOW);
    }

    if (inputString[8] == '1') {
      digitalWrite(41, HIGH);
    }
    else if (inputString[8] == '0') {
      digitalWrite(41, LOW);
    }

    if (inputString[9] == '1') {
      digitalWrite(39, HIGH);
    }
    else if (inputString[9] == '0') {
      digitalWrite(39, LOW);
    }

    posicaoGarra += inputString[1];
    posicaoGarra += inputString[2];
    posicaoGarra += inputString[3];

    M4.write(posicaoGarra.toInt());

    inputString = "";
    posicaoGarra = "";
    stringComplete = false;
  }
*/
  delay(50);
}

/*

void serialEvent() {

  while (Serial.available()) {

    digitalWrite(LED_BUILTIN, HIGH);

    char inChar = (char)Serial.read();    // Lê o novo byte
    inputString += inChar;                // Adiciona o novo byte na String completa
    if (inChar == 'B') {                  // Se o novo byte for 'B'
      stringComplete = true;              // Finaliza aquela palavra
    }
  }

  digitalWrite(LED_BUILTIN, LOW);
}

void SetupControle() {

  // Setup dos pinos do controle, já checando erros
  PS2_Error = Controle.config_gamepad(pin_PS2_CLK, pin_PS2_CMD, pin_PS2_ATT, pin_PS2_DAT, PS2_PRESSURE, PS2_RUMBLE);
}

void LeituraControle() {

  Controle.read_gamepad(false, PS2_Vibrate);    // Ler controle

  if (Controle.NewButtonState()) {
    if (Controle.Button(PSB_RED))     Serial.println("Circulo");
    if (Controle.Button(PSB_GREEN))   Serial.println("Triangulo");
    if (Controle.Button(PSB_BLUE))    Serial.println("X");
    if (Controle.Button(PSB_START))   Serial.println("Start");
    if (Controle.Button(PSB_SELECT))  Serial.println("Select");
  }

  if (Controle.Button(PSB_L1))        Serial.println("L1");
  if (Controle.Button(PSB_R1))        Serial.println("R1");
  if (Controle.Button(PSB_PAD_UP))    Serial.println("UP");
  if (Controle.Button(PSB_PAD_RIGHT)) Serial.println("RIGHT");
  if (Controle.Button(PSB_PAD_LEFT))  Serial.println("LEFT");
  if (Controle.Button(PSB_PAD_DOWN))  Serial.println("DOWN");
}

void SetupMPU6050() {
  Wire.begin();                          // Inicializa a bilioteca Wire
  Wire.beginTransmission(MPU);           // Inicia a transmissão para o endereço do MPU
  Wire.write(0x6B);                      // Escreve o comando de inicialização
  Wire.write(0);                         // Limpa o canal de escrita
  Wire.endTransmission(true);            // Finaliza a transmissão
}

void LeituraMPU6050() {
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
}*/
