#include <Arduino.h>

// Definição dos pinos
const int PINO_ANALOGICO = A0;  // Pino AO do sensor conectado ao A0
const int PINO_DIGITAL = 2;     // Pino DO do sensor conectado ao pino 2

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
  
  // Configura os pinos
  pinMode(PINO_ANALOGICO, INPUT);
  pinMode(PINO_DIGITAL, INPUT);
  
  // Mensagem inicial
  Serial.println("=== SENSOR DE UMIDADE DO SOLO ===");
  Serial.println("Iniciando leituras...");
  Serial.println("AO -> A0, DO -> Pino 2");
  Serial.println("================================");
  delay(2000);
}

void loop() {
  // Lê o valor analógico (0-1023)
  leituraAnalogica = analogRead(PINO_ANALOGICO);
  
  // Lê o valor digital (0 ou 1)
  leituraDigital = digitalRead(PINO_DIGITAL);
  
  // Converte para tensão (0-5V)
  tensao = leituraAnalogica * (5.0 / 1023.0);
  
  // Calcula percentual de umidade (0-100%)
  // Mapeia o valor analógico para percentual
  percentualUmidade = map(leituraAnalogica, VALOR_SECO, VALOR_UMIDO, 0, 100);
  
  // Limita o percentual entre 0 e 100
  percentualUmidade = constrain(percentualUmidade, 0, 100);
  
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
  
  // Interpretação da umidade
  Serial.print("Status: ");
  if (percentualUmidade >= 70) {
    Serial.println("Solo MUITO ÚMIDO");
  } else if (percentualUmidade >= 40) {
    Serial.println("Solo ÚMIDO");
  } else if (percentualUmidade >= 20) {
    Serial.println("Solo MODERADAMENTE SECO");
  } else {
    Serial.println("Solo SECO - PRECISA REGAR!");
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

