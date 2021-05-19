#include <Adafruit_NeoPixel.h>
#include <TimeLib.h>
#include <EEPROM.h>
#include <avr/wdt.h>

Adafruit_NeoPixel strip = Adafruit_NeoPixel(6, 5, NEO_GRB + NEO_KHZ800);

int enable1pin = 7;//Pinos CHIP ENABLE.
int enable2pin = 16;
int enable3pin = 11;
int enable4pin = 15;
int enable5pin = 14;
int enable6pin = 3;

int current1pin = A11;//Pinos leitores de corrente.
int current2pin = A1;
int current3pin = A8;
int current4pin = A3;
int current5pin = A10;
int current6pin = A5;

int Vbatt1pin = A4;//Pinos leitores de tensao.
int Vbatt2pin = A0;
int Vbatt3pin = A7;
int Vbatt4pin = A2;
int Vbatt5pin = A9;
int Vbatt6pin = A6;
char leitura_tempo_A[1] = {0};
char leitura_tempo[13] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

int ligatodos = 1;

int brightness = 10;//Intensidade dos leds
int brightness1 = 0;
int brightness2 = 0;
int setLED = 0;

float corrente[6] = {0, 0, 0, 0, 0, 0}; // Vetores de corrente e tensao
float tensao[6] = {0, 0, 0, 0, 0, 0};
float mandatensao[6] = {0, 0, 0, 0, 0, 0};

int b[6] = {0, 0, 0, 0, 0, 0}; // Auxiliar de estado da bateria (b = 0 desconectada, b = 1 carregando, b = 2 carregada).

float Vbatt = 0; // Tensao de bateria carregada.
float readyfactor = 0; //Fator de bateria carregada.
float mal_contato[6] = {0, 0, 0, 0, 0, 0};
int k = 0;// Auxiliares FOR
int j = 0;
int i = 0;

unsigned long millis_inicio = 0;
unsigned long millis_real = 0;

int recebe_codigo = 0;

//Comandos do supervisorio.
char slot[2] = {0, 0};
char comando[2] = {0, 0};
char valor[4] = {0, 0, 0, 0};
char recebeslot[3] = {0, 0, 0};

int Stop = 0;
int Stopblink = 0;
int Readyblink[6] = {100, 100, 100, 100, 100, 100};

char codigo[12] = {'C', 'H', '1', '9', '2', '0', 'B', 'G', '0', '8', '6', '1'};

unsigned long Tstartbatt[6] = {0, 0, 0, 0, 0, 0}; //Tempo do sistema em segundos de quando a bateria foi conectada.
unsigned long Treadybatt[6] = {0, 0, 0, 0, 0, 0}; //Tempo do sistema em segundos em que a bateria ficou pronta.
unsigned long Tbattout[6] = {0, 0, 0, 0, 0, 0}; //Tempo do sistema em segundos em que a bateria foi retirada.

int enablepin[6] = {enable1pin, enable2pin, enable3pin, enable4pin, enable5pin, enable6pin};
int Vbattpin[6] = {Vbatt1pin, Vbatt2pin, Vbatt3pin, Vbatt4pin, Vbatt5pin, Vbatt6pin};
int currentpin[6] = {current1pin, current2pin, current3pin, current4pin, current5pin, current6pin};

int espera = 20 ; //Delay do loop FOR. (default 20)

float media[50];//Calcula media das tensoes.

unsigned long debounce_bat[6] = {0, 0, 0, 0, 0, 0};

float soma = 0;

float leitura_tensao_carregando = 0;

char msg_485[200];

float Vbatt_check = 0;
unsigned long desliga_temperatura = 0;

int send_wait = 5;

#define RS485_DIR_PIN     2 // Pino RS485.

unsigned long conta_baterias;

int timeout = 200;

float valor_coeficiente;

float AV[6] = {0.00486010, 0.00484400, 0.00484240, 0.00486520, 0.00484920, 0.00486620}; //Coeficientes calibradores de tensao.
float LV[6] = {0.00323550, 0.01776540, 0.01809350, 0.00011390, 0.01229750, 0.00047071};

float AC[6] = {0.00418370, 0.00420180, 0.00441330, 0.00394320, 0.00413990, 0.00431180}; //Coeficientes calibradores de corrente.
float LC[6] = {0.10214840, 0.09629940, 0.07288610, 0.13815130, 0.09800260, 0.09006490};

int calib = 0;

int recebe_calibracao = 0;

void setup() {
  strip.begin();
  strip.show(); // Inicializa todos os leds 'off'

  analogReference(EXTERNAL); //Setando para referencia analogica externa

  pinMode(enable1pin, OUTPUT);// Pinos buck Switch
  pinMode(enable2pin, OUTPUT);
  pinMode(enable3pin, OUTPUT);
  pinMode(enable4pin, OUTPUT);
  pinMode(enable5pin, OUTPUT);
  pinMode(enable6pin, OUTPUT);

  pinMode(current1pin, INPUT); //Pinos leitores de corrente
  pinMode(current2pin, INPUT);
  pinMode(current3pin, INPUT);
  pinMode(current4pin, INPUT);
  pinMode(current5pin, INPUT);
  pinMode(current6pin, INPUT);

  pinMode(Vbatt1pin, INPUT); // Pinos leitores de tensao
  pinMode(Vbatt2pin, INPUT);
  pinMode(Vbatt3pin, INPUT);
  pinMode(Vbatt4pin, INPUT);
  pinMode(Vbatt5pin, INPUT);
  pinMode(Vbatt6pin, INPUT);

  pinMode(RS485_DIR_PIN, OUTPUT);             //Set RS485 direction pin as output
  digitalWrite(RS485_DIR_PIN, LOW);           //Set RS485 chip as receiver

  Serial.begin(9600);
  Serial1.begin(9600);

  slot[0] = EEPROM.read(0);//Recolhe os parametros setados na EEPROM.
  slot[1] = EEPROM.read(1);

  Vbatt = EEPROM.get(2, Vbatt);

  setLED = EEPROM.read(20);
  brightness = EEPROM.read(21);

  for (i = 0; i < 6; i++) { // Desliga todos os carregadores.
    digitalWrite(enablepin[i], HIGH);
  }

  // Apresentacao
  colorWipe(strip.Color(41, 255, 255), 30);
  colorWipe(strip.Color(0, 0, 0), 30);
  colorWipe(strip.Color(41, 255, 255), 30);
  colorWipe(strip.Color(0, 0, 0), 30);
  colorWipe(strip.Color(41, 255, 255), 30);
  colorWipe(strip.Color(0, 0, 0), 30);

  if (setLED == 0) {
    brightness1 = brightness;
    brightness2 = 0;
  }
  else {
    brightness2 = brightness;
    brightness1 = 0;
  }

  for (i = 0; i < 6; i++) { //Capta da EEPROM os dados de calibracao.
    AV[i] = EEPROM.get((30 + i * 4), AV[i]);
  }
  for (i = 0; i < 6; i++) {
    LV[i] = EEPROM.get((60 + i * 4), LV[i]);
  }
  for (i = 0; i < 6; i++) {
    AC[i] = EEPROM.get((90 + i * 4), AC[i]);
  }
  for (i = 0; i < 6; i++) {
    LC[i] = EEPROM.get((120 + i * 4), LC[i]);
  }

  for (i = 0; i < 12; i++) {
    codigo[i] = EEPROM.get((150 + i), codigo[i]);
  }

  // wdt_enable(WDTO_8S);

  //Vbatt = 4.08;
  //brightness = 35;
}

void loop() {

  for (k = 0; k < 3000; k++) { // Loop operacional (verifica se recebeu dados, monitora se baterias foram conectadas e manda os dados para placa de comunicacao).

    // wdt_reset();

    if (Stop == 1) {
      if (k == Stopblink) {
        for (i = 0; i < 6; i++) {
          b[i] = 0;
          strip.setPixelColor(i, brightness , 0, 0);
        }
        strip.show();
        delay(1000);
        Stopblink = (k + 100);
      }
    }

    for (j = 0; j < 6; j++) { // le tensao e verifica se a bateria foi retirada, se sim, seta os leds para desligado.
      tensao[j] = analogRead(Vbattpin[j]);

      //Serial.print("Bateria ");
      //Serial.print(j + 1);
      //Serial.print(": ");
      //Serial.println(tensao[j]);

      if (tensao[j] < 500) {
        digitalWrite(enablepin[j], HIGH);
        if ((b[j] == 2) && ((now() - Tstartbatt[j]) > 300)) {
          Tbattout[j] = now();
          conta_baterias++;
        }
        mandatensao[j] = 0;
        b[j] = 0; strip.setPixelColor(j, 0, 0, 0); strip.show();
      }
    }




    for (i = 0; i < 6; i++) { // Verifica se alguma bateria foi conectada e julga a cor do led que deve ser aceso, alem de ligar o carregador correspondente.
      if (tensao[i] > 500 && b[i] == 0) {
        b[i] = 1;// Status da bateria  = carregando(b = 1).

        tensao[i] = ((AV[i] * tensao[i]) + LV[i]); //Calibra as tensoes.

        Tstartbatt[i] = now(); // Anota o tempo do sistema em segundos que a bateria foi conectada.
        mandatensao[i] = tensao[i];

        if (ligatodos == 1) {
          digitalWrite(enablepin[i], LOW); delay(100);
        }
      }

      //Serial.print("Bateria ");
      //Serial.print(i + 1);
      //Serial.print(": ");
      //Serial.println(digitalRead(enablepin[i]));

      if (b[i] == 1) {
        strip.setPixelColor(i, 0, brightness1, brightness2); //Seta cores dos LEDs
      }
      if (b[i] == 2) {
        strip.setPixelColor(i, 0, brightness2, brightness1);
        if ((now() - Treadybatt[i]) > 300) {
          if ((Readyblink[i] - k) > 50) {
            strip.setPixelColor(i, 0 , 0, 0);
          }
          if ((Readyblink[i] - k) < 1) { //Pisca quando a bateria está pronta.
            Readyblink[i] = (k + 100);
          }
        }
        if (b[i] == 3) {
          strip.setPixelColor(i, brightness, brightness2, brightness1);
        }
      }
    }

    if (calib == 0) {

      if (k == 200) {
        lecorrente();
      }
      if (k == 2700) {
        for (i = 0; i < 6; i++) {
          digitalWrite(enablepin[i], HIGH); delay(50);
        }
      }
      if (k == 2998) {
        letensao();
      }
    }

    if ((now() - desliga_temperatura) > 600) {
      ligatodos = 1;
    }

    comunicacao();//Le as seriais e trata a comunicacao;

    delay(espera);// Delay para aumentar o tempo entre as medidas.
    strip.show();
  }

  if (calib == 0) {

    Stopblink = 0; //seta variavel Stopblink
    for (i = 0; i < 6; i++) {
      Readyblink[i] = 100;
    } //seta variavel Readyblink

    if (setLED == 0) { //Troca as cores padrao dos leds.
      brightness1 = brightness;
      brightness2 = 0;
    }
    else {
      brightness2 = brightness;
      brightness1 = 0;
    }

    for (j = 0; j < 6; j++) { //Liga quando existe bateria e a tensao dela e menor que a tensao especificada.
      if ((mandatensao[j] < Vbatt) && (ligatodos == 1)) {
        if (b[j] == 1 || b[j] == 2 || b[j] == 3) {
          digitalWrite(enablepin[j], LOW); delay(100);
        }
      }
      // Leitura de Tensao da bateria no durante o Carregamento para avaliacao de mal contato .Lucas Oliveira e André Cunha . 29/09/2017

      leitura_tensao_carregando = ((AV[j] * analogRead(Vbattpin[j])) + LV[j]);

      if (mandatensao[j] > Vbatt) {
        if ((now() - Tstartbatt[j]) > 65) {
          if (b[j] == 1 || b[j] == 3) {
            Treadybatt[j] = now(); b[j] = 2;
          } //Status da bateria b = 2 (carregada).
        }
      }
      if ((corrente[j] < 1.5 ) && (b[j] == 1) && (leitura_tensao_carregando < 4.25 )) {
        if ( mal_contato [j] == 0) {
          mal_contato[j] = millis();
        }
        if ((millis() - mal_contato[j] ) > 300000 ) {
          b[j] = 3;
        }

      }
      if ((corrente[j] > 1.5 ) && (b[j] == 3)) {
        b[j] = 1;
        mal_contato[j] = 0;
      }

    }
  }
}

void letensao() {

  for (i = 0; i < 6; i++) {
    if (b[i] == 1 || b[i] == 2 || b[i] == 3) {
      for (j = 0; j < 50; j++) { // Le a tensao na bateria 50 vezes.
        media[j] = analogRead(Vbattpin[i]);
      }
      for (j = 0; j < 50; j++) { //Faz a media.
        soma = soma + media[j];
        //Serial.println(soma);
      }
      soma = (soma / 50);
      tensao[i] = ((AV[i] * soma) + LV[i]); //Calibra as tensoes.
      mandatensao[i] = tensao[i];
      soma = 0;
    }
  }
}

void lecorrente() {

  for (i = 0; i < 6; i++) {
    for (j = 0; j < 50; j++) { // Le a corrente na bateria 50 vezes.
      media[j] = analogRead(currentpin[i]);
    }
    for (j = 0; j < 50; j++) { //Faz a media.
      soma = soma + media[j];
    }
    soma = (soma / 50);
    corrente[i] = ((AC[i] * soma) + LC[i]);
    soma = 0;
  }
}

void comunicacao() {

  if (Serial.available() > 0 || Serial1.available() > 0) { //Entra na comunicacao.

    if (Serial.available()) {
      for (i = 0; i < 3; i++) {
        recebeslot[i] = Serial.read();
        delay(send_wait);
      }
      for (i = 0; i < 2; i++) {
        comando[i] = Serial.read();
        delay(send_wait);
      }
      for (i = 0; i < 4; i++) {
        valor[i] = Serial.read();
        delay(send_wait);
      }
    }

    if (Serial1.available()) {
      for (i = 0; i < 3; i++) {
        recebeslot[i] = Serial1.read();
        delay(send_wait);
      }
      for (i = 0; i < 2; i++) {
        comando[i] = Serial1.read();
        delay(send_wait);
      }
      for (i = 0; i < 4; i++) {
        valor[i] = Serial1.read();
        delay(send_wait);
      }

    }



    //leitura_485();

    if (recebeslot[0] == 'A' && valor[3] == 'B') {
      //Serial.print(recebeslot[1]);
      //Serial.print(recebeslot[2]);
      //Serial.print(comando[0]);
      //Serial.print(comando[1]);
      //Serial.print(valor[0]);
      //Serial.print(valor[1]);
      //Serial.print(valor[2]);
      //Serial.print(valor[3]);
      //Serial.print("fim comando rasp\n");

      if (((recebeslot[1] - 48) == slot[0] || recebeslot[1] == 'P') && ((recebeslot[2] - 48) == slot[1] || recebeslot[2] == 'L')) {
        //Serial.print("aceitei o commando\n");

        /*if (comando[0] == '0' && comando[1] == '0') { //Desliga todos os carregadores
           for (i = 0; i < 6; i++) {
             digitalWrite(enablepin[k], LOW);
             delay(50);
             strip.setPixelColor(i, brightness , 0, 0);
             b[k] = 0;
           }
           strip.show();
           //if (calib == 0) {
            // delay(500);
           //}
           Stop = 1;
           Stopblink = 0;
           ligatodos = 0;
           k = 2999;
           for(i = 0; i < 6; i++){
             b[i] = 4;
           }

          }*/

        if (comando[0] == '0' && comando[1] == '1') { //Liga todos os carregadores
          ligatodos = 1;
          for (i = 0; i < 6; i++) {
            strip.setPixelColor(i, brightness , brightness, brightness);
            b[i] = 0;
          }
          strip.show();
          calib = 0;
          k = 2999;
          Stop = 0;
          delay(1000);
          for (i = 0; i < 6; i++) {
            b[i] = 1;
          }
        }


        if (comando[0] == '0' && comando[1] == '2') { //Muda a intensidade dos LEDS
          // Inserido o intertravamento para mudança da intensidade do LED , no qual é necessário que a intensidade seja no mínimo 017. Lucas Olveira 22/09/2017
          int intensidade_led = ((valor[0] - 48) * 100 + (valor[1] - 48) * 10 + (valor[2] - 48));

          if ( (intensidade_led > 17) ) {
            EEPROM.write(21, ((valor[0] - 48) * 100 + (valor[1] - 48) * 10 + (valor[2] - 48)));
            brightness = EEPROM.read(21);
            Serial.println(brightness);
            for (i = 0; i < 6; i++) {
              strip.setPixelColor(i, brightness , brightness, brightness);
            }
            strip.show();
            k = 2999;
            delay(1000);
          }
        }


        if (comando[0] == '0' && comando[1] == '3') { //Manda tensao e corrente.

          digitalWrite(RS485_DIR_PIN, HIGH);//Seta pino RS485 para mandar mensagem.
          delay(send_wait);
          Serial1.print('A');
          delay(send_wait);
          Serial1.print(';');
          /* for (i = 0; i < 12; i++) {
             Serial1.print(codigo[i]);
             delay(send_wait);
            }
            Serial1.print(';');
            delay(send_wait);
            Serial1.print(slot[1]);
            delay(send_wait);*/
          for (i = 0; i < 6; i++) {
            delay(send_wait);
            Serial1.print(mandatensao[i], 2);
            delay(send_wait);
            Serial1.print(';');
          }
          for (i = 0; i < 6; i++) {
            delay(send_wait);
            Serial1.print(corrente[i], 2);
            delay(send_wait);
            Serial1.print(';');
          }
          delay(send_wait);
          //  Serial1.print(now());
          //  delay(send_wait);
          //  Serial1.print(';');
          delay(send_wait);
          // for (i = 0; i < 6; i++) {
          //   Serial1.print(b[i]); delay(send_wait); Serial1.print(';'); delay(send_wait);
          // }
          Serial1.print(conta_baterias);
          delay(send_wait);
          Serial1.print(';');
          delay(send_wait);
          Serial1.print('B');
          delay(send_wait);
          //Serial1.print(';');
          // delay(send_wait);
          digitalWrite(RS485_DIR_PIN, LOW);//Seta pino RS485 como receptor.

          Serial.print('A');
          for (i = 0; i < 6; i++) {
            Serial.print(mandatensao[i], 2);
            Serial.print(';');
          }
          for (i = 0; i < 6; i++) {
            Serial.print(corrente[i], 2);
            Serial.print(';');
          }
          Serial.print(conta_baterias);
          Serial.print(';');
          Serial.println('B');
          conta_baterias = 0;
        }

        if (comando[0] == '0' && comando[1] == '4') { //Status das baterias.
          digitalWrite(RS485_DIR_PIN, HIGH);
          delay(10);
          Serial1.print('A');
          delay(send_wait);
          for (i = 0; i < 6; i++) {
            Serial1.print(b[i]); delay(send_wait); Serial1.print(';'); delay(send_wait);
          }

          for (i = 0; i < 6; i++) {
            Serial1.print(Tstartbatt[i]); delay(send_wait); Serial1.print(';'); delay(send_wait);
          }

          for (i = 0; i < 6; i++) {
            Serial1.print(Treadybatt[i]); delay(send_wait); Serial1.print(';'); delay(send_wait);
          }

          for (i = 0; i < 6; i++) {
            Serial1.print(Tbattout[i]); delay(send_wait); Serial1.print(';'); delay(send_wait);
          }
          delay(send_wait);
          Serial1.print('B');
          delay(10);
          digitalWrite(RS485_DIR_PIN, LOW);

          Serial.print('A');
          for (i = 0; i < 6; i++) {
            Serial.print(b[i]); Serial.print(';');
          }
          Serial.println();

          for (i = 0; i < 6; i++) {
            Serial.print(Tstartbatt[i]); Serial.print(';');
          }
          Serial.println();

          for (i = 0; i < 6; i++) {
            Serial.print(Treadybatt[i]); Serial.print(';');
          }
          Serial.println();

          for (i = 0; i < 6; i++) {
            Serial.print(Tbattout[i]); Serial.print(';');
          }
          Serial.println('B');
          Serial.println();
        }

        if (comando[0] == '0' && comando[1] == '6') { //Muda o endereço do slot.
          int ID_eeprom = ( valor[1] - '0') + ( valor[2] - '0');
          if ((ID_eeprom >= 1) && (ID_eeprom <= 10)) {

            EEPROM.write(0, (valor[1] - 48));
            slot[0] = EEPROM.read(0);
            EEPROM.write(1, (valor[2] - 48));
            slot[1] = EEPROM.read(1);
            Serial.println(slot);
          }
        }

        if (comando[0] == '0' && comando[1] == '7') { //Muda tensao das baterias.
          Vbatt_check = ((valor[0] - 48) * 100 + (valor[1] - 48) * 10 + (valor[2] - 48));
          Vbatt_check = Vbatt_check / 100;


          // Inserido Intertravamento de segurança para mudança da tensão de carga .É necessário que a tensão seja > 3,3 < 4,4 . Lucas Oliveira 22/09/2017
          if ( (Vbatt_check > 3.2 ) && (Vbatt_check < 4.5 )) {
            Serial.println(Vbatt_check);
            Vbatt = Vbatt_check;
            EEPROM.put(2, Vbatt);
            for (i = 0; i < 3; i++) {
              for (j = 0; j < 6; j++) {
                strip.setPixelColor(j, 0, brightness2, brightness1); strip.show();
              }
              delay(50);
              for (j = 0; j < 6; j++) {
                strip.setPixelColor(j, 0, 0, 0); strip.show();
              }
              delay(100);
            }
          }


          for (j = 0; j < 6; j++) { // Verifica as cores de status dos led
            if ((b[j] == 1) || (b[j] == 2)) {
              if (mandatensao[j] > Vbatt) {
                Treadybatt[j] = now();
                strip.setPixelColor(j, 0, brightness2, brightness1);
                b[j] = 2;
              }
              else {
                strip.setPixelColor(j, 0, brightness1, brightness2); b[j] = 1; Tstartbatt[j] = now();
              }
              strip.show();
            }
          }
        }

        if (comando[0] == '1' && comando[1] == '2') {
          asm volatile (" jmp 0");
        }

        if (comando[0] == '1' && comando[1] == '7') { //Muda padrao dos LEDS
          EEPROM.write(20, (valor[2] - 48));
          setLED = EEPROM.read(20);
          k = 2999;
          Serial.println(setLED);
        }

        if (comando[0] == '2' && comando[1] == '1') { //Desliga os carregadores para controle de temperatura.
          ligatodos = 0;
          desliga_temperatura = now();
          for (i = 0; i < 6; i++) {
            digitalWrite(enablepin[i], HIGH);
            delay(50);
          }
        }
        if (comando[0] == '2' && comando[1] == '2') { //Liga os carregadores para controle de temperatura.
          ligatodos = 1;
        }


        if (comando[0] == '2' && comando[1] == '4') { //Manda contagem de baterias.
          digitalWrite(RS485_DIR_PIN, HIGH);   //Seta pino RS485 para mandar mensagem.
          delay(send_wait);
          Serial1.print('A');
          delay(send_wait);
          Serial1.print(conta_baterias);
          delay(send_wait);
          Serial1.print('B');
          delay(send_wait);
          digitalWrite(RS485_DIR_PIN, LOW);//Seta pino RS485 como receptor.
          delay(send_wait);
          conta_baterias = 0;
        }

        if (comando[0] == '8' && comando[1] == '0') { //Printa os valores da calibracao e o codigo da charger.

          for (i = 0; i < 6; i++) {
            Serial.println(AV[i], 8);
          }
          Serial.println();
          for (i = 0; i < 6; i++) {
            Serial.println(LV[i], 8);
          }
          Serial.println();
          for (i = 0; i < 6; i++) {
            Serial.println(AC[i], 8);
          }
          Serial.println();
          for (i = 0; i < 6; i++) {
            Serial.println(LC[i], 8);
          }
          Serial.println();
          for (i = 0; i < 12; i++) {
            Serial.print(codigo[i]);
          }
          Serial.println();
        }

        if (comando[0] == '8' && comando[1] == '1') { // Ajuste  de hora da placa Charger . Lucas Oliveira 22/09/2017


          for (i = 0; i < 1; i++) {

            leitura_tempo_A[i] = Serial1.read();
            delay(send_wait);
          }
          for (i = 0; i < 11; i++) {
            leitura_tempo[i] = Serial1.read();
            delay(send_wait);
          }
          /*for(i = 0; i < 13; i++){
            leitura_tempo[i] = Serial1.read();
            delay(send_wait);}*/

          if (leitura_tempo_A[0] == 'A' && leitura_tempo[10] == 'B') {
            conta_baterias = 0;
            adjustTime(-now());
            delay(100);
            unsigned long ajuste_hora = ((leitura_tempo[0] - 48) * 1000000000ul + (leitura_tempo[1] - 48) * 100000000ul + (leitura_tempo[2] - 48) * 10000000ul + (leitura_tempo[3] - 48) * 1000000ul + (leitura_tempo[4] - 48) * 100000ul + (leitura_tempo[5] - 48) * 10000ul + (leitura_tempo[6] - 48) * 1000ul + (leitura_tempo[7] - 48) * 100ul + (leitura_tempo[8] - 48) * 10ul + (leitura_tempo[9] - 48));
            adjustTime(ajuste_hora);

            for ( j = 0; j < 6 ; j ++) {
              Tstartbatt[j] = now();
            }
          }
        }

        // Verifica o TimeStamp Configurado na placa Charger
        if (comando[0] == '8' && comando[1] == '2') {
          Serial.println(now());
        }
        // Versão do Software
        if (comando[0] == '8' && comando[1] == '3') {
          Serial.println("Versão 2.0 STBC-60 Manaus");
        }

        if (comando[0] == '8' && comando[1] == '4') { // recebe código da placa.
          recebe_codigo = 1;
          unsigned long inicio = millis();
          unsigned long passando = millis();
          while ((passando - inicio) < 10000) {
            passando = millis();

            if (Serial.available() > 0) {

              msg_485[0] = Serial.read();

              if (msg_485[0] == 'A') {
                for (i = 1; i < 14; i++) {

                  msg_485[i] = Serial.read();
                  //Serial.println(msg_485[i]);
                  if (msg_485[i] == 'Z') {
                    i = 13;
                  }
                }
              }
              if (msg_485[0] == 'A' && msg_485[13] == 'Z') {

                for (i = 1; i < 13; i++) {
                  //Serial.println(msg_485[i]);
                  codigo[i - 1] = msg_485[i];
                  //Serial.println(codigo[i-1]);
                }

                for (i = 0; i < 13; i++) { //Salva os valores do codigo na EEPROM.
                  EEPROM.write((150 + i), codigo[i]);
                }
                recebe_codigo = 0;
              }
            }
          }
        }

        if (comando[0] == '8' && comando[1] == '5') { //Muda tensao das baterias com poulling.
          Vbatt_check = ((valor[0] - 48) * 100 + (valor[1] - 48) * 10 + (valor[2] - 48));
          Vbatt_check = Vbatt_check / 100;


          // Inserido Intertravamento de segurança para mudança da tensão de carga .É necessário que a tensão seja > 3,3 < 4,4 . Lucas Oliveira 22/09/2017
          if ( (Vbatt_check > 3.2 ) && (Vbatt_check < 4.5 )) {
            digitalWrite(RS485_DIR_PIN, HIGH);//Seta pino RS485 para mandar mensagem.
            delay(10);
            Serial1.print('A');
            delay(send_wait);
            Serial1.print('P');
            delay(send_wait);
            Serial1.print('L');
            delay(send_wait);
            Serial1.print('0');
            delay(send_wait);
            Serial1.print('7');
            delay(send_wait);
            Serial1.print(valor[0]);
            delay(send_wait);
            Serial1.print(valor[1]);
            delay(send_wait);
            Serial1.print(valor[2]);
            delay(send_wait);
            Serial1.print('B');
            delay(10);
            digitalWrite(RS485_DIR_PIN, LOW);
            Serial.println(Vbatt_check);
            Vbatt = Vbatt_check;
            EEPROM.put(2, Vbatt);
            for (i = 0; i < 3; i++) {
              for (j = 0; j < 6; j++) {
                strip.setPixelColor(j, 0, brightness2, brightness1); strip.show();
              }
              delay(50);
              for (j = 0; j < 6; j++) {
                strip.setPixelColor(j, 0, 0, 0); strip.show();
              }
              delay(100);
            }
          }


          for (j = 0; j < 6; j++) { // Verifica as cores de status dos led
            if ((b[j] == 1) || (b[j] == 2) || (b[j] == 3)) {
              if (mandatensao[j] > Vbatt) {
                Treadybatt[j] = now();
                strip.setPixelColor(j, 0, brightness2, brightness1);
                b[j] = 2;
              }
              else {
                strip.setPixelColor(j, 0, brightness1, brightness2); b[j] = 1; Tstartbatt[j] = now();
              }
              strip.show();
            }
          }
        }


        if (comando[0] == '9' && comando[1] == '1') { // recebe os coeficientes e coloca na EEPROM.
          unsigned long inicio = millis();
          unsigned long passando = millis();
          while ((passando - inicio) < 10000) {
            passando = millis();
            //recebe_calibracao = 1;
            //while (recebe_calibracao == 1) {

            if (Serial.available() > 0) {

              msg_485[0] = Serial.read();

              if (msg_485[0] == 'A') {
                for (i = 1; i < 220; i++) {
                  msg_485[i] = Serial.read();
                  if (msg_485[i] == 'B') {
                    i = 217;
                  }
                }
              }
              if (msg_485[0] == 'A' && msg_485[217] == 'B') {

                for (i = 0; i < 220; i++) {
                  Serial.println(msg_485[i]);
                }

                for (i = 0; i < 4; i++) { //Salva os valores dos coeficientes na EEPROM.
                  if (i == 0) {
                    k = 1;
                    for (j = 0; j < 6; j++) {
                      valor_coeficiente = (msg_485[k] - '0') * 10000000ul + (msg_485[k + 1] - '0') * 1000000ul + (msg_485[k + 2] - '0') * 100000ul + (msg_485[k + 3] - '0') * 10000ul + (msg_485[k + 4] - '0') * 1000ul + (msg_485[k + 5] - '0') * 100ul + (msg_485[k + 6] - '0') * 10ul + (msg_485[k + 7] - '0');

                      AV[j] = (valor_coeficiente / 10000000);
                      Serial.println(msg_485[k + 8]);
                      if (msg_485[k + 8] == '1') {

                        AV[j] = -AV[j];
                      }
                      EEPROM.put((30 + j * 4), AV[j]);
                      k = k + 9;
                    }
                  }
                  if (i == 1) {
                    k = 49 + 6;
                    for (j = 0; j < 14; j++) {
                      valor_coeficiente = (msg_485[k] - '0') * 10000000ul + (msg_485[k + 1] - '0') * 1000000ul + (msg_485[k + 2] - '0') * 100000ul + (msg_485[k + 3] - '0') * 10000ul + (msg_485[k + 4] - '0') * 1000ul + (msg_485[k + 5] - '0') * 100ul + (msg_485[k + 6] - '0') * 10ul + (msg_485[k + 7] - '0');
                      LV[j] = (valor_coeficiente / 10000000);
                      if (msg_485[k + 8] == '1') {
                        LV[j] = -LV[j];
                      }
                      EEPROM.put((60 + j * 4), LV[j]);
                      k = k + 9;
                    }
                  }
                  if (i == 2) {
                    k = 97 + 12;
                    for (j = 0; j < 6; j++) {
                      valor_coeficiente = (msg_485[k] - '0') * 10000000ul + (msg_485[k + 1] - '0') * 1000000ul + (msg_485[k + 2] - '0') * 100000ul + (msg_485[k + 3] - '0') * 10000ul + (msg_485[k + 4] - '0') * 1000ul + (msg_485[k + 5] - '0') * 100ul + (msg_485[k + 6] - '0') * 10ul + (msg_485[k + 7] - '0');
                      AC[j] = (valor_coeficiente / 10000000);
                      if (msg_485[k + 8 ] == '1') {
                        AC[j] = -AC[j];
                      }
                      EEPROM.put((90 + j * 4), AC[j]);
                      k = k + 9;
                    }
                  }
                  if (i == 3) {
                    k = 145 + 18;
                    for (j = 0; j < 6; j++) {
                      valor_coeficiente = (msg_485[k] - '0') * 10000000ul + (msg_485[k + 1] - '0') * 1000000ul + (msg_485[k + 2] - '0') * 100000ul + (msg_485[k + 3] - '0') * 10000ul + (msg_485[k + 4] - '0') * 1000ul + (msg_485[k + 5] - '0') * 100ul + (msg_485[k + 6] - '0') * 10ul + (msg_485[k + 7] - '0');
                      LC[j] = (valor_coeficiente / 10000000);
                      if (msg_485[k + 8] == '1') {
                        LC[j] = -LC[j];
                      }
                      EEPROM.put((120 + j * 4), LC[j]);
                      k = k + 9;
                    }
                    recebe_calibracao = 0;
                  }
                }
              }
              // }

            }
          }
        }



        if (comando[0] == '9' && comando[1] == '5') {
          Serial.print("Tensao: ");
          Serial.print(Vbatt);
          Serial.println(';');
          Serial.print("LED :");
          Serial.print(brightness);
          Serial.println(';');
          Serial.print("Slot :");
          i = (slot[0] + 48);
          Serial.write(i);
          i = (slot[1] + 48);
          Serial.write(i);
          Serial.println();
          //Serial.println("passei aqui");
        }

        if (comando[0] == '9' && comando[1] == '8') {
          digitalWrite(RS485_DIR_PIN, HIGH);
          delay(10);
          Serial1.print('A');
          delay(send_wait);
          for (i = 0; i < 12; i++) {
            Serial1.print(codigo[i]);
            delay(send_wait);
          }
          delay(send_wait);
          Serial1.print('B');
          delay(10);
          digitalWrite(RS485_DIR_PIN, LOW);
          for (i = 0; i < 12; i++) {
            Serial.print(codigo[i]);
            delay(send_wait);
          }
        }


        if (comando[0] == '9' && comando[1] == '9') { //Calibração
          calib = 1;

          for (i = 0; i < 6; i++) {
            for (k = 0; k < 50; k++) { // Le a tensao na bateria 50 vezes.
              media[k] = analogRead(Vbattpin[i]);
              //delay(2);
            }
            for (k = 0; k < 50; k++) { //Faz a media.
              soma = soma + media[k];
            }
            soma = (soma / 50);
            mandatensao[i] = soma;
            soma = 0;
          }

          for (i = 0; i < 6; i++) {
            Serial.print(mandatensao[i], 2);
            Serial.print(';');
          }

          for (i = 0; i < 6; i++) {
            for (k = 0; k < 50; k++) { // Le a corrente na bateria 50 vezes.
              media[k] = analogRead(currentpin[i]);
              //delay(2);
            }
            for (k = 0; k < 50; k++) { //Faz a media.
              soma = soma + media[k];
            }
            soma = (soma / 50);
            corrente[i] = soma;//Calibra as correntes.
            soma = 0;
          }

          for (i = 0; i < 6; i++) {
            Serial.print(corrente[i], 2);
            Serial.print(';');
          }
        }

        if (comando[0] == '7' && comando[1] == '7') {
          digitalWrite(RS485_DIR_PIN, HIGH);//Seta pino RS485 para mandar mensagem.
          delay(10);
          Serial1.print('A');
          delay(send_wait);
          Serial1.print('P');
          delay(send_wait);
          Serial1.print('L');
          delay(send_wait);
          Serial1.print('7');
          delay(send_wait);
          Serial1.print('7');
          delay(send_wait);
          Serial1.print('0');
          delay(send_wait);
          Serial1.print('0');
          delay(send_wait);
          Serial1.print('0');
          delay(send_wait);
          Serial1.print('B');
          delay(10);
          digitalWrite(RS485_DIR_PIN, LOW);
          rainbowCycle(20);
          colorWipe(strip.Color(255, 0, 0), 50);
          colorWipe(strip.Color(0, 255, 0), 50);
          colorWipe(strip.Color(0, 0, 255), 50);
          theaterChase(strip.Color(127, 127, 127), 50);
          theaterChase(strip.Color(127, 0, 0), 50);
          theaterChase(strip.Color(0, 0, 127), 50);
          theaterChase(strip.Color(0, 127, 0), 50);
        }
        for (i = 0; i < 3; i++) {
          recebeslot[i] = 0;
        }
        for (i = 0; i < 2; i++) {
          comando[i] = 0;
        }
        for (i = 0; i < 4; i++) {
          valor[i] = 0;
        }
        Serial.flush();
      }
    }
  }

}


/*void leitura_485() {

  if (Serial1.available() > 0) {
    i = 0;
    msg_485[i] = Serial1.read();

    if (msg_485[i] == 'A') {

      millis_inicio = millis();
      millis_real = millis();

      while (timeout > (millis_real - millis_inicio)) {
        millis_real = millis();
        i++;
        msg_485[i] = Serial1.read();

        if (msg_485[i] == 'B') {
          break;
        }
        delay(5);
      }
        for(i = 0; i < 3; i++){
          recebeslot[i] = 0;
         }
          for(i = 0; i < 2; i++){
          comando[i] = 0;
         }
          for(i = 0; i < 4; i++){
          valor[i] = 0;
          }





    }

  }


  }*/



void colorWipe(uint32_t c, uint8_t wait) {//Apresentacao.
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}



void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

void theaterChase(uint32_t c, uint8_t wait) {
  for (int j = 0; j < 10; j++) { //do 10 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, c);  //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
