/************************************************************************************************************************
                                                  SENAI "Roberto Mange"
                             Bruno Camargo - Lucas Melaré - Rafael Falcaro - Rubens Lima
                                                         3STMI
                                                         2021
************************************************************************************************************************/


//***********************************************************************************************************************
// Inclusão das Bibliotecas e Header dos pinos da placa
#include <StepperMotor.h>                 // Biblioteca customizada para os Motores de Passo
#include <PS2X_lib.h>                     // Biblioteca do controle de PlayStation 2 sem fio
#include <CustomServo.h>                  // Biblioteca para motores Servo
#include <Wire.h>                         // Biblioteca para uso do Giroscópio MPU6050
#include "PinoutConfig.h"                 // Header de configuração dos pinos da placa Arduino Mega


//***********************************************************************************************************************
// Criação dos Objetos
StepperMotor MOTOR[4];      // 0, Base, Ombro, Antebraço
CustomServo  SERVO[5];      // 0, 0, 0, 0, Garra
PS2X         PS2;           // Controle de PlayStation 2 sem fio


//***********************************************************************************************************************
// Declaração de Variáveis

// Comunicação Serial
String cmd = "";                          // String que armazena os dados vindos da serial
bool stringComplete = false;              // Flag de finalização de uma palavra

// Controle PS2 sem fio
int  PS2_Error    = 0;                    // Tratamento de erro de inicialização da comunicação com o controle sem fio
byte PS2_Vibrate  = 0;                    // Velocidade da vibração do controle (0 a 255)
bool PS2_PRESSURE = true;                 // Ler pressões?
bool PS2_RUMBLE   = true;                 // Ler rumbles?

// Sensor Giroscópio
const int MPU = 0x68;                     // Endereco I2C do MPU6050
int aX, aY, aZ, Tmp, gX, gY, gZ;          // Variaveis para armazenar valores dos sensores (a=Aceleração | Tmp=Temperatura | g=Rotação)


//***********************************************************************************************************************
// Setup do Sistema
void setup() {

  // Comunicação Serial
  SetupSerial();

  // Motores
  SetupMotors();

  // Sensor Giroscópio
  SetupMPU6050();

  // Controle PS2 sem fio
  SetupPS2();
}


//***********************************************************************************************************************
// Rotina principal
void loop() {

  // Se receber algum caracter na Serial
  if (stringComplete) {
    // Seleção das funções de acordo com o comando inicial
    if      (cmd.charAt(0) == '#') MoveMotor(cmd);    // Exemplo: abs #1=25   inc #2^-30   jog #1j+   hold #1j/   kill #1k
    else if (cmd.charAt(0) == '&') MoveCS(cmd);       // Exemplo:
    else if (cmd.charAt(0) == 'M') SetParameter(cmd); // Exemplo: M1.JogSpeed=180 M1.HomeOffset=-40

    else Serial.println("Comando inválido");          // Se não for nenhuma das opções, retorna mensagem de erro
    cmd = "";                                         // Limpa a variável que armazena os comandos
    stringComplete = false;                           // Define como falso a flag de String Completa
  }
}
