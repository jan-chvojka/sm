/*********
  Jan Chvojka
  -------------------------------------------------------------
  Sušený Maso Sensor/Controller

  Complete project details at https://github.com/jan-chvojka/smsc
*********/

// NEZAPOMENOUT PŘEJMENOVAT default.config.h NA sm.config.h
// A NASTAVIT DLE POTŘEBY
#include "sm.config.h"

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

// Vlastnostii displeje
#define SCREEN_WIDTH 128 // šířka displeje v px
#define SCREEN_HEIGHT 64 // výška displeje v px

// Nastavení adresy senzoru
#define BME280_ADRESA (0x76)

// Pokud chceme zasílat údaje po wifi.
#define WIFI true
#define WIFI_CONNECTION_INTERVAL 5 * 1000
#define WIFI_RECONNECTION_INTERVAL 30 * 1000
#define WIFI_SETUP_CONNECTION_ATTEMPTS 3

#if DEBUG
#define MEASUREMENT_INTERVAL 5 * 1000
#else
#define MEASUREMENT_INTERVAL 20 * 1000
#endif

// Deklarace SSD1306 displeje připojeného na I2C (SDA, SCL piny)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
// Deklarace senzoru
Adafruit_BME280 bme;

float measurements[2];
int WifiConnectionAttempt = 0;

unsigned long timeNow = 0;
unsigned long timeLastWifiConnectionAttempt = 0;

// typedef struct Buttons {
//   const byte pin = D3;
//   const int debounce = 100;
//   const unsigned long shortPress = 100;
//   const unsigned long doublePress = 600;
//   const unsigned long  longPress = 1000;

//   unsigned long counter = 0;
//   int shortPressAmount = 0;
//   bool previousState = NOT_PRESSED;
//   bool currentState;
// } Button;

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
  // aktuální čas od spuštění
  timeNow = millis()/1000; 

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
  #if WIFI
    return;
  #endif

  
  
  WiFi.disconnect();
  delay(500);

  timeLastWifiConnectionAttempt = millis()/1000; 

  #if DEBUG
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
  WifiConnectionAttempt = 0;
  Serial.println("[setupWifi] end");
  # endif
}

void checkWifiConnection() {

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
  #if WIFI
    return;
  #endif

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
