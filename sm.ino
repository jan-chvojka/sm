/*********
  Jan Chvojka
  Complete project details at https://github.com/jan-chvojka/tsm
*********/
// Pokud chcete použít sériový výstup a sledovat
// dění v programu pomocí serial monitoru, nastavte
// false.
#define DEBUG true

// Knihovny pro komunikaci po I2C
#include <Wire.h>
#include <SPI.h>
// Knihovna pro wifi
#include <ESP8266WiFi.h> 
// Knihovna pro displej
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// Knihovna pro senzor teploty
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// OBSAHUJE HESLO PRO WIFI, přejmenujte default.secret.h na sm.secret.h
#include "sm.config.h"

// Vlastnostii displeje
#define SCREEN_WIDTH 128 // šířka displeje v px
#define SCREEN_HEIGHT 64 // výška displeje v px

// Nastavení adresy senzoru
#define BME280_ADRESA (0x76)

// Pokud chceme zasílat údaje po wifi.
#define WIFI true
#define WIFI_CONNECTION_INTERVAL 5 * 1000
#define WIFI_SETUP_CONNECTION_ATTEMPTS 3

#define MEASUREMENT_INTERVAL 10 * 1000

// Deklarace SSD1306 displeje připojeného na I2C (SDA, SCL piny)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
// Deklarace senzoru
Adafruit_BME280 bme;

float measurements[2];
int WifiConnectionAttempt = 0;

/**
 * Inicializace počítače.
 */
void setup() {
  setupDebug();
  setupDisplay();
  displayWelcomeMessage();
  setupSensor();
  setupWifi();
}

/**
 * Hlavní smyčka programu.
 */
void loop() {
  // naměříme hodnoty
  getMeasurements();
  // zobrazíme je na displeji
  displayMeasurements(String(measurements[0]), String(measurements[1]));
  //
  displayWiFiStatus();
  // uspíme počtač na definovanou dobu
  delay(MEASUREMENT_INTERVAL);
}

/**
 * Vrací naměřené hodnoty.
 * Na indexu 0 je teplota.
 * Na indexu 1 je vlhkost.
 */
void getMeasurements() {
  
  #ifdef DEBUG
  display.println("[getMeasurements] begin");
  # endif

  measurements[0] = bme.readTemperature();
  measurements[1] = bme.readHumidity();

  #ifdef DEBUG
  display.println("[getMeasurements] C: " + String(measurements[0]));
  display.println("[getMeasurements] H: " + String(measurements[1]));
  # endif

  #ifdef DEBUG
  display.println("[getMeasurements] end");
  # endif
}

/**
 * Zobrazí hodnoty na displeji.
 */
void displayMeasurements(String temperature, String humidity) {
  display.clearDisplay();
  
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  // Display static text
  display.println("C: " + temperature);

  display.setCursor(0, 16);
  display.println("H: " + humidity);
  
  display.display(); 
}

void setupDebug() {
  #ifdef DEBUG
  Serial.begin(9600);
  // Počkáme na připojení serial monitoru.
  delay(6000);
  #endif
}

void setupSensor() {
  if (!bme.begin(BME280_ADRESA)) {
    displayErrorMessage("BME280: error");
    delay(5000);
    #ifdef DEBUG
    Serial.println("[setupSensor] BME280 senzor nenalezen, zkontrolujte zapojeni!");
    #endif
    while (1);
  }  
}

void setupDisplay() {
  #ifdef DEBUG
  Serial.println("[setupDisplay] begin");
  # endif
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    #ifdef DEBUG
    Serial.println(F("[setupDisplay] SSD1306 allocation failed"));
    # endif
    for(;;);
  }  

  #ifdef DEBUG
  Serial.println("[setupDisplay] end");
  # endif
}

void setupWifi() {
  #ifdef DEBUG
  Serial.println("[setupWifi] begin");
  Serial.println("[setupWifi] ssid: " + String(WIFI_SSID));
  Serial.println("[setupWifi] pass: " + String(WIFI_PASSWORD));
  # endif
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    WifiConnectionAttempt++;
    char message[128];
    snprintf(message, sizeof(message), "WiFi: connecting [%i]", WifiConnectionAttempt);
    displayErrorMessage(String(message));
    delay(WIFI_CONNECTION_INTERVAL);
    
    if(WifiConnectionAttempt >= WIFI_SETUP_CONNECTION_ATTEMPTS
      && WiFi.status() != WL_CONNECTED) {
      #ifdef DEBUG
      Serial.println("[setupWifi] could not connect to wifi.");
      # endif
      break;  
    }
    #ifdef DEBUG
    Serial.print(".");
    # endif
  }
  #ifdef DEBUG
  Serial.println();
  Serial.println("[setupWifi] end");
  # endif
}

void displayWelcomeMessage() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  // Display static text
  display.println("SM");
  display.setTextSize(1);
  display.setCursor(0, 16);
  display.println("v 1.0");
  
  display.display(); 

  delay(3000);
}

void deleteLine(int y1, int y2) {
  int x,y;
  for (y=y1; y<=y2; y++)
  {
     for (x=0; x<127; x++)
     {
      display.drawPixel(x, y, BLACK);
     }
  }
  display.display(); 
}

void displayErrorMessage(String message) {
  display.setTextSize(1);
  deleteLine(48,63);
  
  display.setCursor(0, 48);
  display.println(message);

  display.display(); 
}

void displayWiFiStatus() {
  deleteLine(48,63);
  display.setTextSize(1);
  display.setCursor(0, 48);
  if(WiFi.status() == WL_CONNECTED) 
  {
    display.println("WiFi: OK");
  }
  else 
  {
    display.println("WiFi: Error");
  }
  display.display(); 
}
