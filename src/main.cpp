#include <Arduino.h>

// Definição dos pinos
const int PINO_ANALOGICO = A0;  // Pino AO do sensor conectado ao A0
const int PINO_DIGITAL = 12;     // Pino DO do sensor conectado ao pino 12

// Pinos do display de 10 segmentos (2-11)
const int DISPLAY_PINOS[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

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
  
  // Configura o pino do sensor
  pinMode(PINO_ANALOGICO, INPUT);
  pinMode(PINO_DIGITAL, INPUT);
  
  // Configura os pinos do display de 10 segmentos como saída
  for (int i = 0; i < 10; i++) {
    pinMode(DISPLAY_PINOS[i], OUTPUT);
    digitalWrite(DISPLAY_PINOS[i], LOW); // Garante que iniciem apagados
  }
  
  // Mensagem inicial
  Serial.println("=== SENSOR DE UMIDADE DO SOLO COM DISPLAY 10 SEGMENTOS ===");
  Serial.println("Iniciando leituras...");
  Serial.println("AO -> A0, DO -> Pino 12");
  Serial.println("Display: Pinos 2-11 (cada segmento = 10% umidade)");
  Serial.println("========================================");
  delay(2000);
}

void atualizarDisplay(int umidade) {
  // Apaga todos os segmentos primeiro
  for (int i = 0; i < 10; i++) {
    digitalWrite(DISPLAY_PINOS[i], LOW);
  }
  
  // Calcula quantos segmentos devem estar ligados
  // Cada 10% de umidade = 1 segmento
  int segmentosLigados = umidade / 10;
  
  // Garante que não exceda 10 segmentos
  if (segmentosLigados > 10) {
    segmentosLigados = 10;
  }
  
  // Liga os segmentos necessários
  for (int i = 0; i < segmentosLigados; i++) {
    digitalWrite(DISPLAY_PINOS[i], HIGH);
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
  
  // Atualiza o display baseado na umidade
  atualizarDisplay(percentualUmidade);
  
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
  
  // Mostra quantos segmentos estão ligados
  int segmentosAtivos = percentualUmidade / 10;
  if (segmentosAtivos > 10) segmentosAtivos = 10;
  Serial.print("Segmentos ligados: ");
  Serial.print(segmentosAtivos);
  Serial.println("/10");
  
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
    Serial.println("Solo SECO - PRECISA REGAR! (1-2 segmentos)");
  } else if (percentualUmidade <= 40) {
    Serial.println("Solo LIGEIRAMENTE SECO (3-4 segmentos)");
  } else if (percentualUmidade <= 60) {
    Serial.println("Solo ÚMIDO (5-6 segmentos)");
  } else if (percentualUmidade <= 80) {
    Serial.println("Solo BEM ÚMIDO (7-8 segmentos)");
  } else {
    Serial.println("Solo MUITO ÚMIDO (9-10 segmentos)");
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

