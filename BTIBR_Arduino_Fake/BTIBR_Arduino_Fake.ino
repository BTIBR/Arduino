/************************************************************************************************************************
                                                  SENAI "Roberto Mange"
                             Bruno Camargo - Lucas Melaré - Rafael Falcaro - Rubens Lima
                                                         3STMI
                                                         2021
************************************************************************************************************************/


//***********************************************************************************************************************
// Inclusão das Bibliotecas e Header dos pinos da placa
#include "modbus_rtu_slave.h"
#include "PinoutConfig.h"


//***********************************************************************************************************************
// Declaração de Variáveis
int weightMeasure = 0;

// Geração dados Célula de Carga


//***********************************************************************************************************************
// Setup do Sistema
void setup() {

  // Comunicação Serial
  Serial.begin(9600);

  // Comunicação Modbus
  modbus_init(9600, RE_DE, SLAVE_ADDRS);
  modbus_update_holding(0, 0);

}


//***********************************************************************************************************************
// Rotina principal
void loop() {
  weightMeasure = random(95000, 105000);

  modbus_update_holding(1, weightMeasure);
  modbus_check();

  Serial.println(weightMeasure);
}
