#include <LiquidCrystal.h>
 
//Define os pinos que serão utilizados para ligação ao display
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
 
void setup()
{
  //Define o número de colunas e linhas do LCD
  lcd.begin(16, 2);
}
 
void loop()
{
  //Limpa a tela
  lcd.clear();
  //Posiciona o cursor na coluna 0, linha 0;
  lcd.setCursor(0, 0);
  //Envia o texto entre aspas para o LCD
  lcd.print("TESTE PRE CARGA");
   lcd.setCursor(0, 1);
  //Envia o texto entre aspas para o LCD
  lcd.print("FUNCIONA");
  delay(1000);
}

//linha 19 vc tem q alterar pra leitura da célula
