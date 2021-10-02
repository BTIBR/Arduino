void SetupMotors() {

  // Motor 1
  MOTOR[1].SetPins(pin_M1_FAULT, pin_M1_DIR, pin_M1_SLEEP, pin_M1_RESET, pin_M1_MODE2, pin_M1_MODE1, pin_M1_ENABLE, pin_M1_MODE0, pin_M1_STEP, pin_M1_CS, pin_M1_INC, pin_M1_UD);
  MOTOR[1].SetMode(0);                          // Definindo o modo de funcionamento do Motor: 0=FullStep, 1=HalfStep, 2=1/4, 3=1/8, 4=1/16, 5=1/32
  MOTOR[1].Dir         = 0;                     // Sentido de Giro: 0->CW / 1->CCW
  MOTOR[1].HomeOffset  = 0;                     // Offset de posição [°]
  //MOTOR[1].JogSpeed    = 180;                 // Setpoint de velocidade de movimento [°/s]
  MOTOR[1].JogSpeed    = 90;       // Counts
  MOTOR[1].JogTa       = 1;                     // Tempo de aceleração [s]
  MOTOR[1].JogTd       = 1;                     // Tempo de desaceleração [s]
  MOTOR[1].MaxPos      = 0;                     // Posição Máxima permitida [°]
  MOTOR[1].MinPos      = 0;                     // Posição Mínima permitida [°]
  MOTOR[1].ScaleFactor = 1 / 1;                 // Fator de escala para a movimentação
  MOTOR[1].Home();                              // Movimento automático até a posição de Home

  // Motor 2
  MOTOR[2].SetPins(pin_M2_FAULT, pin_M2_DIR, pin_M2_SLEEP, pin_M2_RESET, pin_M2_MODE2, pin_M2_MODE1, pin_M2_ENABLE, pin_M2_MODE0, pin_M2_STEP, pin_M2_CS, pin_M2_INC, pin_M2_UD);
  MOTOR[2].SetMode(0);                          // Definindo o modo de funcionamento do Motor: 0=FullStep, 1=HalfStep, 2=1/4, 3=1/8, 4=1/16, 5=1/32
  MOTOR[2].Dir         = 0;                     // Sentido de Giro: 0->CW / 1->CCW
  MOTOR[2].HomeOffset  = 0;                     // Offset de posição [°]
  //MOTOR[2].JogSpeed    = 180;                 // Setpoint de velocidade de movimento [°/s]
  MOTOR[2].JogSpeed    = 90;       // Counts
  MOTOR[2].JogTa       = 1;                     // Tempo de aceleração [s]
  MOTOR[2].JogTd       = 1;                     // Tempo de desaceleração [s]
  MOTOR[2].MaxPos      = 0;                     // Posição Máxima permitida [°]
  MOTOR[2].MinPos      = 0;                     // Posição Mínima permitida [°]
  MOTOR[2].ScaleFactor = 1 / 1;                 // Fator de escala para a movimentação
  MOTOR[2].Home();                              // Movimento automático até a posição de Home

  // Motor 3
  MOTOR[3].SetPins(pin_M3_FAULT, pin_M3_DIR, pin_M3_SLEEP, pin_M3_RESET, pin_M3_MODE2, pin_M3_MODE1, pin_M3_ENABLE, pin_M3_MODE0, pin_M3_STEP, pin_M3_CS, pin_M3_INC, pin_M3_UD);
  MOTOR[3].SetMode(0);                          // Definindo o modo de funcionamento do Motor: 0=FullStep, 1=HalfStep, 2=1/4, 3=1/8, 4=1/16, 5=1/32
  MOTOR[3].Dir         = 0;                     // Sentido de Giro: 0->CW / 1->CCW
  MOTOR[3].HomeOffset  = 0;                     // Offset de posição [°]
  //MOTOR[3].JogSpeed    = 180;                 // Setpoint de velocidade de movimento [°/s]
  MOTOR[3].JogSpeed    = 90;       // Counts
  MOTOR[3].JogTa       = 1;                     // Tempo de aceleração [s]
  MOTOR[3].JogTd       = 1;                     // Tempo de desaceleração [s]
  MOTOR[3].MaxPos      = 0;                     // Posição Máxima permitida [°]
  MOTOR[3].MinPos      = 0;                     // Posição Mínima permitida [°]
  MOTOR[3].ScaleFactor = 1 / 1;                 // Fator de escala para a movimentação
  MOTOR[3].Home();                              // Movimento automático até a posição de Home

  // Motor 4
  SERVO[4].SetPin(pin_M4);                      // Definindo o pino de saída PWM do Servo
  SERVO[4].Dir        = 0;                      // Sentido de Giro: 0->CW / 1->CCW
  SERVO[4].HomeOffset = 0;                      // Offset de posição do Servo [°]
  SERVO[4].JogSpeed   = 50;                     // Setpoint de velocidade de movimento do Servo [1-100%]
  SERVO[4].JogTa      = 1;                      // Tempo de aceleração do Servo [s]
  SERVO[4].JogTd      = 1;                      // Tempo de desaceleração do Servo [s]
  SERVO[4].MaxPos     = 180;                    // Posição Máxima permitida do Servo [°]
  SERVO[4].MinPos     = 0;                      // Posição Mínima permitida do Servo [°]
  SERVO[4].Home();                              // Movimento automático até a posição de Home
}

void MoveMotor(String _cmd) {

  _cmd.remove(0, 1);                                // Remove o primeiro caracter do comando
  String numMotor_S = _cmd.substring(0, 1);         // Extrai o número do motor que o comando foi dado
  int numMotor_I = numMotor_S.toInt();              // Converte o número do motor para Int
  String moveType_S = _cmd.substring(1, 2);         // Extrai o tipo de movimento a ser realizado (absoluto / incremental / jog / kill)

  if (moveType_S == "=") {
    _cmd.remove(0, 2);                              // Remove os caracteres iniciais,
    String movement_S = _cmd;                       // deixando apenas o valor do movimento
    int movement_I = movement_S.toInt();            // Converte o movimento para Int
    if (numMotor_I == 4) SERVO[numMotor_I].MoveTo(movement_I);  // Move Servo em modo absoluto
    else MOTOR[numMotor_I].MoveTo(movement_I);                  // Move Stepper em modo absoluto
  }

  else if (moveType_S == "^") {
    _cmd.remove(0, 2);                              // Remove os caracteres iniciais,
    String movement_S = _cmd;                       // deixando apenas o valor do movimento
    int movement_I = movement_S.toInt();            // Converte o movimento para Int
    if (numMotor_I == 4) SERVO[numMotor_I].MoveInc(movement_I); // Move Servo em modo absoluto
    else MOTOR[numMotor_I].MoveInc(movement_I);                 // Move Stepper em modo absoluto
  }

  else if (moveType_S == "j") {
    String dir_S = _cmd.substring(2, 3);            // Extrai a direção do movimento
    if (dir_S == "+") MOTOR[numMotor_I].Jog(0);     // Move em Jog Positivo
    else if (dir_S == "-") MOTOR[numMotor_I].Jog(1);// Ativa o freio (holding torque)
    else MOTOR[numMotor_I].Brake();                 // Move em Jog Negativo
  }

  else if (moveType_S == "k") {
    MOTOR[numMotor_I].Kill();                       // Interrompe o controle do Motor
  }

  else Serial.println("Comando inválido");          // Se não for nenhuma das opções, retorna mensagem de erro
}

void MoveCS(String _cmd) {
  _cmd.remove(0, 1);
  Serial.println("CS");
}

void SetParameter(String _cmd) {
  _cmd.remove(0, 1);                                  // Remove o caractere M

  String numMotor_S = _cmd.substring(0, 1);           // Extrai o número do motor que o comando foi dado
  int numMotor_I = numMotor_S.toInt();                // Converte o número do motor para Int

  _cmd.remove(0, 2);                                  // Remove os caracteres número do motor + .
  int indexEq = _cmd.indexOf("=");                    // Procura pela posição do caractere =

  String parameter = _cmd.substring(0, indexEq);      // Copia da String _cmd só a parte do parâmetro
  _cmd.remove(0, indexEq + 1);                        // Remove o parâmetro e o caractere =
  double parameterValue = _cmd.toDouble();            // Extrai o valor do parâmetro a ser configurado

  if (numMotor_I == 4) {
    if      (parameter == "Dir")         SERVO[4].Dir        = parameterValue;
    else if (parameter == "HomeOffset")  SERVO[4].HomeOffset = parameterValue;
    else if (parameter == "JogSpeed")    SERVO[4].JogSpeed   = parameterValue;
    else if (parameter == "JogTa")       SERVO[4].JogTa      = parameterValue;
    else if (parameter == "JogTd")       SERVO[4].JogTd      = parameterValue;
    else if (parameter == "MaxPos")      SERVO[4].MaxPos     = parameterValue;
    else if (parameter == "MinPos")      SERVO[4].MinPos     = parameterValue;
    else Serial.println("Comando inválido");
  }

  else {
    if      (parameter == "Dir")         MOTOR[numMotor_I].Dir         = parameterValue;
    else if (parameter == "HomeOffset")  MOTOR[numMotor_I].HomeOffset  = parameterValue;
    else if (parameter == "JogSpeed")    MOTOR[numMotor_I].JogSpeed    = parameterValue;
    else if (parameter == "JogTa")       MOTOR[numMotor_I].JogTa       = parameterValue;
    else if (parameter == "JogTd")       MOTOR[numMotor_I].JogTd       = parameterValue;
    else if (parameter == "MaxPos")      MOTOR[numMotor_I].MaxPos      = parameterValue;
    else if (parameter == "MinPos")      MOTOR[numMotor_I].MinPos      = parameterValue;
    else if (parameter == "ScaleFactor") MOTOR[numMotor_I].ScaleFactor = parameterValue;
    else Serial.println("Comando inválido");
  }
}
