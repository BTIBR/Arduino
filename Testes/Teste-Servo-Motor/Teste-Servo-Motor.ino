#include <Servo.h>

Servo Garra;

String conteudo;
char caractere;
int pos = 80;
int flag = 0;

void setup() {
  Serial.begin(9600);
  Garra.attach(9);
}

void loop() {
  /*Serial.println("0");
    Garra.write(0);
    delay(2000);
    Serial.println("45");
    Garra.write(45);
    delay(2000);
    Serial.println("90");
    Garra.write(90);
    delay(2000);
    Serial.println("135");
    Garra.write(135);
    delay(2000);
    Serial.println("180");
    Garra.write(180);
    delay(2000);*/

  /*while (Serial.available() > 0) {
    caractere = Serial.read();
    if (caractere != '\n') {
      conteudo.concat(caractere);
    }
    delay(10);
    pos = conteudo.toInt();
    flag = 1;
  }

  Garra.write(pos);

  if (flag == 1) {
    Serial.println(Garra.read());
    conteudo = "";
    flag = 0;
  }*/

  /*
    Garra.write(30);
    delay(2000);
    Garra.write(140);
    delay(2000);*/


    Garra.write(90);

}
