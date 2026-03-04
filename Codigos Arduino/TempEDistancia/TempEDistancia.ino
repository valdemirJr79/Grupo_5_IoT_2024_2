#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <WebServer.h>
#include "html.h"

// Configurações dos Pinos
#define DS18B20PIN 4       // Pino do sensor de temperatura
const int pingPin = 27;    // Pino SIG do sensor Ultrassônico

OneWire oneWire(DS18B20PIN);
DallasTemperature sensor(&oneWire);
WebServer server(80);

// Variáveis Globais
float _temperature = 0;
long _distanceCm = 0;

const char* ssid = "REDEWORK"; 
const char* password = "Acessonet05"; 

void MainPage() {
  server.send(200, "text/html", html_page);
}

// Rota que envia os dados para o site
void readData() {
  // Criamos uma string simples separando os valores por vírgula
  String data = String(_temperature) + "," + String(_distanceCm);
  server.send(200, "text/plain", data);
}

void setup(void){
  Serial.begin(115200);
  
  // WiFi Setup
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){ 
    delay(500); 
    Serial.print("."); 
  }
  
  Serial.println("\nConectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  sensor.begin();

  server.on("/", MainPage);
  server.on("/readData", readData); // Nova rota unificada
  server.begin();
}

void loop(void){
  // 1. Ler Temperatura
  sensor.requestTemperatures(); 
  _temperature = sensor.getTempCByIndex(0);

  // 2. Ler Distância (Ultrassônico)
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  pinMode(pingPin, INPUT);
  long duration = pulseIn(pingPin, HIGH);
  _distanceCm = duration / 29 / 2;

  // Debug no Serial
  Serial.print("Temp: "); Serial.print(_temperature);
  Serial.print("°C | Dist: "); Serial.print(_distanceCm);
  Serial.println("cm");

  server.handleClient();
  delay(500); // Frequência de atualização
}