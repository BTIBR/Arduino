/************************************************************************************************************************
                                                  SENAI "Roberto Mange"
                             Bruno Camargo - Lucas Melaré - Rafael Falcaro - Rubens Lima
                                                         3STMI
                                                         2021
************************************************************************************************************************/

#include <X9C103S.h>

#define pin_M1_CS   44
#define pin_M1_INC  45
#define pin_M1_UD   46

X9C103S POT;





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

// Sensor Giroscópio
const int MPU = 0x68;                     // Endereco I2C do MPU6050
int aX, aY, aZ, Tmp, gX, gY, gZ;          // Variaveis para armazenar valores dos sensores (a=Aceleração | Tmp=Temperatura | g=Rotação)

// Controle PS2 sem fio
int  PS2_Error    = 0;                    // Tratamento de erro de inicialização da comunicação com o controle sem fio
byte PS2_Vibrate  = 0;                    // Velocidade da vibração do controle (0 a 255)
bool PS2_PRESSURE = true;                 // Ler pressões?
bool PS2_RUMBLE   = true;                 // Ler rumbles?

// Comunicação Serial
String cmd = "";                          // String que armazena os dados vindos da serial
bool stringComplete = false;              // Flag de finalização de uma palavra


//***********************************************************************************************************************
// Setup do Sistema
void setup() {

  // Comunicação Serial
  Serial.begin(9600);                     // Inciando a porta de comunicação serial
  Serial.println("Bancada de Teste de Pré-Carga em Fusos de Esfera Integrada com Braço Robótico\n");
  cmd.reserve(20);                        // Reservando até 20 bytes para a palavra de comunicação

  // Motor 1
  MOTOR[1].SetPins(pin_M1_FAULT, pin_M1_DIR, pin_M1_SLEEP, pin_M1_RESET, pin_M1_MODE2, pin_M1_MODE1, pin_M1_ENABLE, pin_M1_MODE0, pin_M1_STEP, pin_M1_CS, pin_M1_INC, pin_M1_UD);
  MOTOR[1].SetMode(0);                          // Definindo o modo de funcionamento do Motor: 0=FullStep, 1=HalfStep, 2=1/4, 3=1/8, 4=1/16, 5=1/32

  // Motor 2
  MOTOR[2].SetPins(pin_M2_FAULT, pin_M2_DIR, pin_M2_SLEEP, pin_M2_RESET, pin_M2_MODE2, pin_M2_MODE1, pin_M2_ENABLE, pin_M2_MODE0, pin_M2_STEP, pin_M2_CS, pin_M2_INC, pin_M2_UD);
  MOTOR[2].SetMode(0);                          // Definindo o modo de funcionamento do Motor: 0=FullStep, 1=HalfStep, 2=1/4, 3=1/8, 4=1/16, 5=1/32

  // Motor 3
  MOTOR[3].SetPins(pin_M3_FAULT, pin_M3_DIR, pin_M3_SLEEP, pin_M3_RESET, pin_M3_MODE2, pin_M3_MODE1, pin_M3_ENABLE, pin_M3_MODE0, pin_M3_STEP, pin_M3_CS, pin_M3_INC, pin_M3_UD);
  MOTOR[3].SetMode(0);                          // Definindo o modo de funcionamento do Motor: 0=FullStep, 1=HalfStep, 2=1/4, 3=1/8, 4=1/16, 5=1/32

  // Motor 4
  SERVO[1].SetPin(pin_M4);                      // Definindo o pino de saída PWM do Servo
  SERVO[1].HomeOffset = 0;                      // Offset de posição do Servo [°]
  SERVO[1].JogSpeed = 50;                       // Setpoint de velocidade de movimento do Servo [1-100%]
  SERVO[1].JogTa = 1;                           // Tempo de aceleração do Servo [s]
  SERVO[1].JogTd = 1;                           // Tempo de desaceleração do Servo [s]
  SERVO[1].MaxPos = 180;                        // Posição Máxima permitida do Servo [°]
  SERVO[1].MinPos = 0;                          // Posição Mínima permitida do Servo [°]
  SERVO[1].Home();                              // Movendo o Servo para a posição de Home

  // Sensor Giroscópio
  SetupMPU6050();                         // Setup do sensor MPU6050

  // Controle PS2 sem fio
  SetupPS2();                             // Setup do controle sem fio de PS2



  POT.SetPins(pin_M1_INC, pin_M1_UD, pin_M1_CS);
  POT.Reset();
  POT.Set(25);
}


//***********************************************************************************************************************
// Rotina principal
void loop() {
  if (stringComplete) {
    // Seleção das funções de acordo com o comando inicial
    if      (cmd.charAt(0) == '#') MoveMotor(cmd);
    else if (cmd.charAt(0) == '&') MoveCS(cmd);
    else if (cmd.charAt(0) == 'M') SetParameter(cmd);
    else if (cmd.charAt(0) == 'J') MoveJog(cmd);

    else if (cmd.charAt(0) == 'K') MOTOR[1].Kill();
    else if (cmd.charAt(0) == 'B') MOTOR[1].Brake();
    else if (cmd.charAt(0) == 'V') SetVelo(cmd);
    
    else Serial.println("Comando inválido");          // Se não for nenhuma das opções, retorna mensagem de erro
    cmd = "";                                         // Limpa a variável que armazena os comandos
    stringComplete = false;                           // Define como falso a flag de String Completa
  }
}
