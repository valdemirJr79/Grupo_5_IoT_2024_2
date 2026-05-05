#include <WiFi.h>
#include <PubSubClient.h>

// Wi-Fi credentials
const char* ssid = "REDEWORK";
const char* password = "Acessonet05";

// MQTT Broker (your Raspberry Pi)
const char* mqtt_server = "192.168.1.8"; // replace with your Pi's IP
const int mqtt_port = 1883;
const char* mqtt_topic = "status/estacionamento";  // topic for this parking spot

#define trigPin 13
#define echoPin 12

// Distance threshold (cm) to consider spot occupied
const int occupiedThreshold = 30; // adjust based on sensor placement

WiFiClient espClient;
PubSubClient client(espClient);

String lastStatus = ""; // track last published state

void setup() {
  Serial.begin(115200);

  // HC-SR04 setup
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Connect to MQTT broker
  client.setServer(mqtt_server, mqtt_port);
  connectMQTT();
}

void connectMQTT() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect("ESP32ParkingSpot")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 2 seconds");
      delay(2000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    connectMQTT();
  }
  client.loop();

  // Measure distance
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;


  // Determine parking spot status
  String status = (distance <= occupiedThreshold) ? "Ocupado" : "Livre";

  // Only publish if status changed
  if (status != lastStatus) {
    Serial.print("Parking spot status changed: ");
    Serial.println(status);

    client.publish(mqtt_topic, status.c_str());
    lastStatus = status; // update last known state
  }

  delay(2000); // update every second
}