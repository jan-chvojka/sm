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

// Knihovna pro displej
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// Knihovna pro senzor teploty
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#if WIFI
#include "wifi.defs.h"
#include "wifi.func.h"
#endif

#include "button.defs.h"

// Deklarace SSD1306 displeje připojeného na I2C (SDA, SCL piny)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
// Deklarace senzoru
Adafruit_BME280 bme;
// Deklarace tlačítka
Button button;

#include "button.func.h"

float measurements[2];
int WifiConnectionAttempt = 0;

unsigned long timeNow = 0;
unsigned long timeLastWifiConnectionAttempt = 0;

/**
 * Inicializace počítače.
 */
void setup() {

  pinMode(D7 , INPUT);

  setupDebug();
  setupDisplay();
  displayWelcomeMessage();
  setupButton();
  setupSensor();
  #ifdef setupWifi
  setupWifi();
  #endif
}

/**
 * Hlavní smyčka programu.
 */

long loopCount = 0;
long loopCountMax = 100000;

void loop() {
  // aktuální čas od spuštění
  timeNow = millis()/1000; 
  
  loopButton();
  // naměříme hodnoty
  // getMeasurements();
  // // zobrazíme je na displeji
  // displayMeasurements(String(measurements[0]), String(measurements[1]));
  // #ifdef displayWiFiStatus
  // displayWiFiStatus();
  // #endif

  // delay(MEASUREMENT_INTERVAL);

  
}

/**
 * Vrací naměřené hodnoty.
 * Na indexu 0 je teplota.
 * Na indexu 1 je vlhkost.
 */
void getMeasurements() {
  
  #if DEBUG
  Serial.println("[getMeasurements] begin");
  #endif

  measurements[0] = bme.readTemperature();
  measurements[1] = bme.readHumidity();

  #if DEBUG
  Serial.println("[getMeasurements] C: " + String(measurements[0]));
  Serial.println("[getMeasurements] H: " + String(measurements[1]));
  # endif

  #if DEBUG
  Serial.println("[getMeasurements] end");
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
  #if DEBUG
  Serial.begin(9600);
  // Počkáme na připojení serial monitoru.
  // while (!Serial) {
  //   ; // wait for serial port to connect. Needed for native USB port only
  // }
  delay(5000);
  Serial.println();
  #endif
}


void setupSensor() {
  #ifdef DEBUG
    Serial.println("[setupSensor] begin");
  #endif
  if (!bme.begin(BME280_ADRESA)) {
    displayErrorMessage("BME280: error");
    delay(5000);
    #ifdef DEBUG
    Serial.println("[setupSensor] BME280 senzor nenalezen, zkontrolujte zapojeni!");
    #endif
    while (1);
  }  
  #ifdef DEBUG
    Serial.println("[setupSensor] end");
  #endif
}

void setupDisplay() {
  #if DEBUG
  Serial.println("[setupDisplay] begin");
  #endif
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    #if DEBUG
    Serial.println(F("[setupDisplay] SSD1306 allocation failed"));
    #endif
    for(;;);
  }  

  #if DEBUG
  Serial.println("[setupDisplay] end");
  #endif
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


