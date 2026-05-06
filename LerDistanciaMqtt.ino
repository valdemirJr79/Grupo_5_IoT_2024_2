#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoOTA.h>

// Wi-Fi credentials
const char* ssid = "REDEWORK";
const char* password = "Acessonet05";

// MQTT Broker
const char* mqtt_server = "192.168.1.8";
const int mqtt_port = 1883;
const char* mqtt_topic = "status/estacionamento";

#define trigPin 13
#define echoPin 12

const int occupiedThreshold = 10;

WiFiClient espClient;
PubSubClient client(espClient);

String lastStatus = "";

// ================= WIFI =================
void connectWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWi-Fi conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

// ================= MQTT =================
void connectMQTT() {
  while (!client.connected()) {
    Serial.print("Conectando ao MQTT...");
    if (client.connect("ESP32ParkingSpot")) {
      Serial.println("conectado");
    } else {
      Serial.print("falhou, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente...");
      delay(2000);
    }
  }
}

// ================= SETUP =================
void setup() {
  Serial.begin(115200);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  connectWiFi();

  client.setServer(mqtt_server, mqtt_port);

  // ================= OTA =================
  ArduinoOTA.setHostname("esp32-estacionamento");

  // (opcional) senha
  // ArduinoOTA.setPassword("123456");

  ArduinoOTA.onStart([]() {
    Serial.println("Iniciando OTA...");
  });

  ArduinoOTA.onEnd([]() {
    Serial.println("\nOTA finalizado!");
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progresso: %u%%\r", (progress / (total / 100)));
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Erro OTA[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });

  ArduinoOTA.begin();

  connectMQTT();
}

// ================= LOOP =================
void loop() {
  // OTA sempre rodando
  ArduinoOTA.handle();

  if (!client.connected()) {
    connectMQTT();
  }
  client.loop();

  // ===== SENSOR =====
  long duration, distance;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;

  String status = (distance <= occupiedThreshold) ? "Vaga ocupada" : "Vaga livre";

  if (status != lastStatus) {
    Serial.print("Status mudou: ");
    Serial.println(status);

    client.publish(mqtt_topic, status.c_str());
    lastStatus = status;
  }

  delay(2000);
}