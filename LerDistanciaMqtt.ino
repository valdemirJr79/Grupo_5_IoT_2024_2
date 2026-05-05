#include <WiFi.h>          // Use <ESP8266WiFi.h> for ESP8266
#include <PubSubClient.h>

// WiFi credentials
const char* ssid = "Redework";
const char* password = "Acessonet05";

// MQTT broker
const char* mqtt_server = "192.168.1.8"; // your broker IP
const int mqtt_port = 1883;
const char* mqtt_topic = "sensor/estacionamento";

// HC-SR04 pins
#define TRIG_PIN 13
#define ECHO_PIN 12

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    
    if (client.connect("ArduinoClient")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
  }
}

float readDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  
  float distance = duration * 0.034 / 2;
  return distance;
}

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  setup_wifi();

  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  float distance = readDistance();

  char msg[50];
  snprintf(msg, 50, "%.2f", distance);

  Serial.print("Distance: ");
  Serial.println(msg);

  client.publish(mqtt_topic, msg);

  delay(2000);
}