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

HTSensor htSensorCurrent;

void mainFSMLoop(bool reset = false)
{
  const uint32_t DISPLAY_ON_TIME = DISPLAY_ON_DURATION;
  static enum { DISPLAY_ON, DISPLAY_OFF, DISPLAY_SLEEP, DISPLAY_WAIT } state = DISPLAY_ON;
  static uint32_t timeLastTransition = 0;
 
  // nastavení defaulního stavu, když potřebujeme
  // například změřit data po stisku tlačítka, abychom
  // tam neměli "starou" hodnotu. Nebo obecně resetovat
  // průběh.
  if (reset)
  { 
    state = DISPLAY_ON;
    // digitalWrite(LED_BUILTIN, LOW);
  }
 
  switch (state)
  {
    case DISPLAY_OFF: 
      Serial.println("OFF");
      display.ssd1306_command(SSD1306_DISPLAYOFF);
      state = DISPLAY_SLEEP;
      break;
    case DISPLAY_SLEEP:   
      break;      
    case DISPLAY_ON:   
      // po zapnutí displeje čeká než vyprší čas a pak se vypne
      Serial.println("ON");
      // display.ssd1306_command(SSD1306_DISPLAYON);
      display.ssd1306_command(SSD1306_DISPLAYON);
      timeLastTransition = millis();
      state = DISPLAY_WAIT;
      break;
    case DISPLAY_WAIT:
      if (millis() - timeLastTransition >= DISPLAY_ON_TIME)
        state = DISPLAY_OFF;
      break;
    default:
      state = DISPLAY_ON;
      break;
  }
}

void reset() {
  sensorFSMLoop(true);
  mainFSMLoop(true);
}

/**
 * Hlavní smyčka programu.
 */
void loop() {

  loopButton();
  // změříme hodnoty na senzoru  
  sensorFSMLoop(false);

  // pokud se liší, zobrazíme
  if(htSensorCurrent.temperature != htSensor.temperature
  || htSensorCurrent.humidity != htSensor.humidity) 
  {
    htSensorCurrent.temperature = htSensor.temperature;
    htSensorCurrent.humidity = htSensor.humidity;

    String t = "C: " + String(htSensorCurrent.temperature);
    String h = "H: " + String(htSensorCurrent.humidity);

    displayLines(t, h);
  }

  mainFSMLoop(false);
  
  #if WIFI
  displayWiFiStatus(displayStatusMessage);
  #endif
}

/**
 * Základní nastavení
 */
void setupDebug() {
  #if SERIAL
  Serial.begin(9600);
  // Počkáme na připojení serial monitoru, abychom chytli 
  // setup výpisy.
  #if DEBUG
  delay(7000);
  #endif
  Serial.println();
  #endif
}


/**
 * Inicializace počítače.
 */
void setup() {
  setupDebug();
  setupDisplay();
  displayWelcomeMessage();
  setupButton(reset);
  setupSensor(displayStatusMessage);
  #if WIFI
  Serial.println("[setup] setupWifi");
  setupWifi(displayStatusMessage);
  #endif
}


