#include <OneWire.h>
#include <DallasTemperature.h>

// GPIO pins
#define SENSOR1_PIN 27
#define SENSOR2_PIN 33

// OneWire instances
OneWire oneWire1(SENSOR1_PIN);
OneWire oneWire2(SENSOR2_PIN);

// DallasTemperature instances
DallasTemperature sensor1(&oneWire1);
DallasTemperature sensor2(&oneWire2);

void setup() {
  Serial.begin(115200);

  // Start sensors
  sensor1.begin();
  sensor2.begin();

  Serial.println("Starting dual temperature reading...");
}

void loop() {

  // Request BOTH temperatures first
  sensor1.requestTemperatures();
  sensor2.requestTemperatures();

  // Read temperatures
  float temp1 = sensor1.getTempCByIndex(0);
  float temp2 = sensor2.getTempCByIndex(0);

  // Print sensor 1
  Serial.print("Sensor 1 (GPIO 27): ");
  if (temp1 != DEVICE_DISCONNECTED_C) {
    Serial.print(temp1);
    Serial.println(" C");
  } else {
    Serial.println("Not detected");
  }

  // Print sensor 2
  Serial.print("Sensor 2 (GPIO 33): ");
  if (temp2 != DEVICE_DISCONNECTED_C) {
    Serial.print(temp2);
    Serial.println(" C");
  } else {
    Serial.println("Not detected");
  }

  Serial.println("----------------");

  delay(2000);
}