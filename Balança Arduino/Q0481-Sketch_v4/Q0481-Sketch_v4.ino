/*
 *   Q0000
 *   AUTOR:   BrincandoComIdeias
 *   LINK:    https://www.youtube.com/brincandocomideias ; https://cursodearduino.net/
 *   COMPRE:  https://www.arducore.com.br/
 *   SKETCH:  Módulo para Arduino HX711
 *   DATA:    04/07/2019
*/

// INCLUSÃO DE BIBLIOTECAS
#include <HX711.h>
#include <PushButton.h>
#include <LiquidCrystal.h>
 

LiquidCrystal lcd(12, 11, 5, 4, 3, 2); //Define os pinos que serão utilizados para ligação ao display

// DEFINIÇÕES DE PINOS
#define pinDT  A1
#define pinSCK  A0
#define pinBotao 9

// DEFINIÇÕES
#define pesoMin 0.010
#define pesoMax 30.0

#define escala -1110000.0f

// INSTANCIANDO OBJETOS
HX711 scale;
PushButton botao(pinBotao);

// DECLARAÇÃO DE VARIÁVEIS  
float medida=0;

void setup() {
  
  lcd.begin(16, 2); //Define o número de colunas e linhas do LCD
  
  Serial.begin(57600);

  scale.begin(pinDT, pinSCK); // CONFIGURANDO OS PINOS DA BALANÇA
  scale.set_scale(escala); // ENVIANDO O VALOR DA ESCALA CALIBRADO

  delay(2000);
  scale.tare(); // ZERANDO A BALANÇA PARA DESCONSIDERAR A MASSA DA ESTRUTURA
  //Serial.println("Setup Finalizado!");
}

void loop() {
 // lcd.clear(); //Limpa a tela
 lcd.setCursor(0, 0); //Posiciona o cursor na coluna 0, linha 0;
  lcd.print("TESTE PRE CARGA");
  botao.button_loop(); // LEITURA DO BOTAO

  if(botao.pressed()){ // QUANDO PRESSIONAR O BOTÃO
    
    scale.power_up(); // LIGANDO O SENSOR
    
    medida = scale.get_units(5); // SALVANDO NA VARIAVEL O VALOR DA MÉDIA DE 5 MEDIDAS
    
    if (medida <= pesoMin ){ // CONFERE SE A MASSA ESTÁ NA FAIXA VÁLIDA
      scale.tare(); // ZERA A BALANÇA CASO A MASSA SEJA MENOR QUE O VALOR MIN
      medida = 0;
      lcd.setCursor(0, 1); //Posiciona o cursor na coluna 0, linha 1;
      lcd.print("Tara Configurada");
    } else if ( medida >= pesoMax ){
      scale.tare(); // ZERA A BALANÇA CASO A MASSA SEJA MAIOR QUE O VALOR MÁX
      medida = 0;
      lcd.setCursor(0, 1); //Posiciona o cursor na coluna 0, linha 1;
      lcd.print("Tara Configurada");
    } else {
      lcd.setCursor(0, 1); //Posiciona o cursor na coluna 0, linha 1;
      lcd.print(medida * 1000);
      lcd.print("G");
      lcd.print(" OU ");
      lcd.print(medida * 9.8);
      lcd.print("N");
    }

    scale.power_down(); // DESLIGANDO O SENSOR
  }
  
}

// IMPLEMENTO DE FUNÇÕES
