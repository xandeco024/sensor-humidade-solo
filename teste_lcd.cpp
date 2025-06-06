#include <Arduino.h>
#include <LiquidCrystal.h>

// LCD nos mesmos pinos
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  Serial.begin(9600);
  Serial.println("Teste básico LCD");
  
  lcd.begin(16, 2);
  delay(1000);
  
  lcd.clear();
  lcd.print("Hello World!");
  lcd.setCursor(0, 1);
  lcd.print("Linha 2 OK");
  
  Serial.println("Mensagens enviadas ao LCD");
}

void loop() {
  // Pisca um cursor para mostrar que está funcionando
  lcd.setCursor(15, 1);
  lcd.print("*");
  delay(500);
  lcd.setCursor(15, 1);
  lcd.print(" ");
  delay(500);
} 