/*********
  Jan Chvojka
  -------------------------------------------------------------
  Sušený Maso Sensor/Controller

  Complete project details at https://github.com/jan-chvojka/smsc
*********/

// NEZAPOMENOUT PŘEJMENOVAT default.config.h NA sm.config.h
// A NASTAVIT DLE POTŘEBY
#include "sm.config.h"

// Knihovny pro komunikaci po I2C (společné)
#include <Wire.h>
#include <SPI.h>

// Knihovna pro displej
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Definice
#include "sensors.defs.h"
#include "button.defs.h"
#if WIFI
#include "wifi.defs.h"
#endif

// Funkce
#include "display.func.h"
#include "button.func.h"
#include "sensors.func.h"
#if WIFI
#include "wifi.func.h"
#endif

unsigned long timeNow = 0;

/**
 * Inicializace počítače.
 */
void setup() {
  setupDebug();
  setupDisplay();
  displayWelcomeMessage();
  setupButton();
  setupSensor(displayStatusMessage);
  #if WIFI
  Serial.println("[setup] setupWifi");
  setupWifi(displayStatusMessage);
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
  
  sensorFSMLoop(false);
  //loopButton();
  // naměříme hodnoty
  // getMeasurements();
  // // zobrazíme je na displeji
  // displayMeasurements(String(measurements[0]), String(measurements[1]));
  #if WIFI
  displayWiFiStatus(displayStatusMessage);
  #endif

  // delay(MEASUREMENT_INTERVAL);
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



void checkWifiConnection() {

}




