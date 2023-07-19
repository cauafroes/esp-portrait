#include <Arduino.h>
#include <ArduinoJson.h>
#include "SPIFFS.h"
#include <HTTPClient.h>
#include <WiFi.h>

const char* ssid = "galaxy";
const char* password = "12344321";

void makeHttpGetRequest(const String& url) {
  HTTPClient http;

  // Send GET request
  http.begin(url);

  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    // Successfully connected to the server
    String payload = http.getString();
    Serial.println("Response payload:");
    Serial.println(payload);

    // Parse the JSON data
    StaticJsonDocument<1024> doc; // Adjust the size depending on your JSON data
    DeserializationError error = deserializeJson(doc, payload);

    // Check for parsing errors
    if (error) {
      Serial.print("JSON parsing failed: ");
      Serial.println(error.c_str());
      return;
    }

    // Extract specific information from the JSON
    String value = doc["whatsapp"].as<String>(); // Replace "key" with the specific JSON key you want to extract
    Serial.print("Value of the key: ");
    Serial.println(value);
  } else {
    Serial.print("HTTP GET request failed with error code: ");
    Serial.println(httpCode);
  }

  // Close the connection and free resources
  http.end();
}

void downloadImage(){
  HTTPClient http;
  String url = "https://froesmhs.com/espcurlimage/image.jpeg";
  Serial.println("Começando o download da imagem");

  http.begin(url);

  int httpCode = http.GET();
  if(httpCode == HTTP_CODE_OK){
    File file = SPIFFS.open("/image2.jpeg", FILE_WRITE);
    if(!file){
      Serial.println("Erro ao abrir o arquivo");
      return;
    }

    Stream* response = http.getStreamPtr();
    while (response->available()){
      file.write(response->read());
    }

    file.close();
    Serial.println("Download concluido");
  } else {
    Serial.println("Erro ao fazer o download");
  }

  http.end();
}

void printFiles() {
  File root = SPIFFS.open("/");

  File file = root.openNextFile();
 
  while(file){
 
      Serial.print("FILE: ");
      Serial.println(file.name());
 
      file = root.openNextFile();
  }
  file.close();
}


void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  if (!SPIFFS.begin(true)) {
    Serial.println("Failed to mount SPIFFS");
    return;
  }
  if(!SPIFFS.format()){
    Serial.println("Erro ao formatar");
    return;
  }

  Serial.println("SPIFFS mounted successfully");

  while (WiFi.status() != WL_CONNECTED){
    delay(2000);
    Serial.println("conectando ao wifi");
  }

  Serial.println("Conectado");

  delay(1000);

  // downloadImage();
  // printFiles();
  makeHttpGetRequest("https://api.clubedeferias.com/api/links");
  Serial.println("Fim do programa");
  esp_deep_sleep_start();
}

void loop() {

}