void MoveStepper(String _cmd) {
  _cmd.remove(0, 1);                                    // Remove o primeiro caracter do comando

  String numMotor_S = _cmd.substring(0, 1);             // Extrai o número do motor que o comando foi dado
  int numMotor_I = numMotor_S.toInt();                  // Converte o número do motor para Int

  String moveType_S = _cmd.substring(1, 2);             // Extrai o tipo de movimento a ser realizado (absoluto / incremental)

  _cmd.remove(0, 2);                                    // Remove os caracteres iniciais,
  String movement_S = _cmd;                             // deixando apenas o valor do movimento
  int movement_I = movement_S.toInt();                  // Converte o movimento para Int

  if (moveType_S == "=")      MOTOR[numMotor_I].MoveTo(movement_I);    // Move em modo absoluto
  else if (moveType_S == "^") MOTOR[numMotor_I].MoveInc(movement_I);   // Move em modo incremental

  Serial.print("Motor[");
  Serial.print(numMotor_I);
  Serial.print("].ActPos = ");
  Serial.print(MOTOR[numMotor_I].ActPos);
  Serial.println();
}

void MoveServo(String _cmd) {
  _cmd.remove(0, 1);                                    // Remove o primeiro caracter do comando

  String numMotor_S = _cmd.substring(0, 1);             // Extrai o número do motor que o comando foi dado
  int numMotor_I = numMotor_S.toInt();                  // Converte o número do motor para Int

  String moveType_S = _cmd.substring(1, 2);             // Extrai o tipo de movimento a ser realizado (absoluto / incremental)

  _cmd.remove(0, 2);                                    // Remove os caracteres iniciais,
  String movement_S = _cmd;                             // deixando apenas o valor do movimento
  int movement_I = movement_S.toInt();                  // Converte o movimento para Int

  if (moveType_S == "=")      SERVO[numMotor_I].MoveTo(movement_I);    // Move em modo absoluto
  else if (moveType_S == "^") SERVO[numMotor_I].MoveInc(movement_I);   // Move em modo incremental

  Serial.print("Servo[");
  Serial.print(numMotor_I);
  Serial.print("].ActPos = ");
  Serial.print(SERVO[numMotor_I].ActPos);
  Serial.println();
}

void MoveCS(String _cmd) {
  _cmd.remove(0, 1);
  Serial.println("CS");
}

void SetParameter(String _cmd) {
  _cmd.remove(0, 1);
  Serial.println("VELO");
}
