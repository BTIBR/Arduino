/************************************************************************************************************************
                                                  SENAI "Roberto Mange"
                             Bruno Camargo - Lucas Melaré - Rafael Falcaro - Rubens Lima
                                                         3STMI
                                                         2021
************************************************************************************************************************/


//***********************************************************************************************************************
// Inclusão das Bibliotecas e Header dos pinos da placa
#include <HX711.h>
#include "modbus_rtu_slave.h"
#include "PinoutConfig.h"


//***********************************************************************************************************************
// Criação dos Objetos
HX711 StrainGauge;


//***********************************************************************************************************************
// Declaração de Variáveis

// Célula de Carga
#define minWeight   0.010
#define maxWeight   30.0
#define scaleFactor -1110000.0f
float weightMeasure = 0;


//***********************************************************************************************************************
// Setup do Sistema
void setup() {

  // Comunicação Serial
  Serial.begin(57600);

  // Célula de Carga
  StrainGauge.begin(pinDT, pinSCK);
  StrainGauge.set_scale(scaleFactor);
  delay(2000);
  StrainGauge.tare();
  StrainGauge.power_up();

  // Comunicação Modbus
  modbus_init(9600, RE_DE, SLAVE_ADDRS);
  modbus_update_holding(0, 0);

}


//***********************************************************************************************************************
// Rotina principal
void loop() {

  weightMeasure = StrainGauge.get_units(5);

  modbus_update_holding(0, weightMeasure);
  modbus_check();

  Serial.println(weightMeasure, 3);

}
