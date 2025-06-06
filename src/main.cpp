#include <Arduino.h>
#include <LiquidCrystal.h>

// Configuração do LCD (RS, EN, D4, D5, D6, D7)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Definição dos pinos
const int PINO_ANALOGICO = A0;  // Pino AO do sensor conectado ao A0
const int PINO_DIGITAL = 6;     // Pino DO do sensor conectado ao pino 6

// Variáveis para armazenar as leituras
int leituraAnalogica = 0;
int leituraDigital = 0;
float tensao = 0.0;
int percentualUmidade = 0;

// Valores de calibração (ajuste conforme necessário)
const int VALOR_SECO = 1023;    // Valor quando solo está seco
const int VALOR_UMIDO = 300;    // Valor quando solo está úmido

void setup() {
  // Inicializa comunicação serial
  Serial.begin(9600);
  Serial.println("Iniciando sistema...");
  
  // Inicializa o LCD
  lcd.begin(16, 2);
  delay(500); // Aguarda estabilizar
  
  Serial.println("LCD inicializado");
  
  // Teste simples do LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TESTE LCD");
  lcd.setCursor(0, 1);
  lcd.print("Funcionando?");
  
  Serial.println("Mensagem de teste enviada ao LCD");
  Serial.println("Se não aparecer texto no LCD, verifique:");
  Serial.println("1. Contraste (potenciômetro no pino V0)");
  Serial.println("2. Conexões dos pinos");
  Serial.println("3. Alimentação 5V do LCD");
  
  // Configura o pino do sensor
  pinMode(PINO_ANALOGICO, INPUT);
  pinMode(PINO_DIGITAL, INPUT);
  
  delay(3000); // Deixa a mensagem de teste por 3 segundos
  
  // Mensagem inicial no LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sensor Umidade");
  lcd.setCursor(0, 1);
  lcd.print("Iniciando...");
  
  // Mensagem inicial no Serial
  Serial.println("=== SENSOR DE UMIDADE DO SOLO COM LCD ===");
  Serial.println("Iniciando leituras...");
  Serial.println("AO -> A0, DO -> Pino 6");
  Serial.println("LCD: Pinos 12,11,5,4,3,2");
  Serial.println("========================================");
  
  delay(2000);
  lcd.clear();
}

void atualizarLCD(int umidade, int valorAnalogico) {
  // Debug no Serial
  Serial.print("Atualizando LCD - Umidade: ");
  Serial.print(umidade);
  Serial.println("%");
  
  // Limpa o LCD
  lcd.clear();
  
  // Linha 1: Mostra a umidade
  lcd.setCursor(0, 0);
  lcd.print("Umidade: ");
  lcd.print(umidade);
  lcd.print("%");
  
  // Linha 2: Status baseado na umidade
  lcd.setCursor(0, 1);
  if (umidade <= 20) {
    lcd.print("Solo SECO!");
  } else if (umidade <= 40) {
    lcd.print("Pouco umido");
  } else if (umidade <= 60) {
    lcd.print("Umido");
  } else if (umidade <= 80) {
    lcd.print("Bem umido");
  } else {
    lcd.print("Muito umido");
  }
}

void loop() {
  // Lê o valor analógico (0-1023)
  leituraAnalogica = analogRead(PINO_ANALOGICO);
  
  // Lê o valor digital (0 ou 1)
  leituraDigital = digitalRead(PINO_DIGITAL);
  
  // Converte para tensão (0-5V quando alimentado com 5V)
  tensao = leituraAnalogica * (5.0 / 1023.0);
  
  // Calcula percentual de umidade (0-100%)
  // Mapeia o valor analógico para percentual
  percentualUmidade = map(leituraAnalogica, VALOR_SECO, VALOR_UMIDO, 0, 100);
  
  // Limita o percentual entre 0 e 100
  percentualUmidade = constrain(percentualUmidade, 0, 100);
  
  // Atualiza o LCD com os dados
  atualizarLCD(percentualUmidade, leituraAnalogica);
  
  // Exibe os dados no monitor serial
  Serial.println("--- LEITURA DO SENSOR ---");
  Serial.print("Valor Analógico: ");
  Serial.println(leituraAnalogica);
  Serial.print("Tensão: ");
  Serial.print(tensao, 2);
  Serial.println(" V");
  Serial.print("Valor Digital: ");
  Serial.println(leituraDigital);
  Serial.print("Umidade: ");
  Serial.print(percentualUmidade);
  Serial.println(" %");
  
  // Ajuda para calibração (sistema 5V)
  Serial.println("--- CALIBRAÇÃO (5V) ---");
  Serial.print("Valor atual: ");
  Serial.print(leituraAnalogica);
  Serial.print(" | Seco configurado: ");
  Serial.print(VALOR_SECO);
  Serial.print(" | Úmido configurado: ");
  Serial.println(VALOR_UMIDO);
  
  // Interpretação da umidade
  Serial.print("Status: ");
  if (percentualUmidade <= 20) {
    Serial.println("Solo SECO - PRECISA REGAR!");
  } else if (percentualUmidade <= 40) {
    Serial.println("Solo LIGEIRAMENTE SECO");
  } else if (percentualUmidade <= 60) {
    Serial.println("Solo ÚMIDO");
  } else if (percentualUmidade <= 80) {
    Serial.println("Solo BEM ÚMIDO");
  } else {
    Serial.println("Solo MUITO ÚMIDO");
  }
  
  // Status do pino digital
  Serial.print("Alerta Digital: ");
  if (leituraDigital == HIGH) {
    Serial.println("Solo seco detectado");
  } else {
    Serial.println("Solo com umidade adequada");
  }
  
  Serial.println("========================");
  Serial.println();
  
  // Aguarda 2 segundos antes da próxima leitura
  delay(2000);
}

