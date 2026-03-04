#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <WebServer.h>
#include "html.h"

#define DS18B20PIN 04                      /* Connect DS18B20 to Pin No D2 of ESP32*/
OneWire oneWire(DS18B20PIN);
DallasTemperature sensor(&oneWire);

WebServer server(80);
float _temperature;

const char* ssid = "REDEWORK";         /*Enter Your SSID*/
const char* password = "Acessonet05"; /*Enter Your Password*/
 
void MainPage() {
  String _html_page = html_page;              /*Read The HTML Page*/
  server.send(200, "text/html", _html_page);  /*Send the code to the web server*/
}

void Temp() {
  String TempValue = String(_temperature);    //Convert it into string
  server.send(200, "text/plane", TempValue);  //Send updated temperature value to the web server
}

void setup(void){
  Serial.begin(115200);               /*Set the baudrate to 115200*/
  WiFi.mode(WIFI_STA);                /*Set the WiFi in STA Mode*/
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  delay(1000);                       /*Wait for 1000mS*/
  while(WiFi.waitForConnectResult() != WL_CONNECTED){Serial.print(".");}
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("Your Local IP address is: ");
  Serial.println(WiFi.localIP());     /*Print the Local IP*/
  sensor.begin();

  server.on("/", MainPage);           /*Display the Web/HTML Page*/
  server.on("/readTemp", Temp);       /*Display the updated Temperature and Humidity value*/
  server.begin();                    /*Start Server*/
  delay(1000);                       /*Wait for 1000mS*/
}

void loop(void){
  sensor.requestTemperatures(); 
  _temperature = sensor.getTempCByIndex(0); /* Read the temperature */
  Serial.print("Temperature = ");
  Serial.print(_temperature);         /* Print Temperature on the serial window */
  Serial.println("ºC");
  server.handleClient(); 
  delay(1000);                       /* Wait for 1000mS */
}