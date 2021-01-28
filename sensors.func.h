// Deklarace Adafruit senzoru
Adafruit_BME280 bme01;
// Deklarace senzoru
HTSensor htSensor;

void setupSensor(void (*displayMessage)(String text)) {
  #ifdef DEBUG
    Serial.println("[setupSensor] begin");
  #endif
  if (!bme01.begin(BME280_ADRESA)) {
    displayMessage("BME280: error");
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

/**
 * Vrací naměřené hodnoty.
 * Na indexu 0 je teplota.
 * Na indexu 1 je vlhkost.
 */
void getMeasurements() {
  #if DEBUG
  Serial.println("[getMeasurements] begin");
  #endif

  htSensor.temperature = bme01.readTemperature();
  htSensor.humidity = bme01.readHumidity();

  #if DEBUG
  Serial.println("[getMeasurements] C: " + String(htSensor.temperature));
  Serial.println("[getMeasurements] H: " + String(htSensor.humidity));
  # endif

  #if DEBUG
  Serial.println("[getMeasurements] end");
  # endif
}

/**
 * https://arduinoplusplus.wordpress.com/2019/07/06/finite-state-machine-programming-basics-part-1/
 * */
void sensorFSMLoop(bool reset = false)
{
  const uint32_t READ_DELAY = MEASUREMENT_INTERVAL;
  static enum { READ_DATA, WAIT_DELAY } state = READ_DATA;
  static uint32_t timeLastTransition = 0;
 
  // nastavení defaulního stavu, když potřebujeme
  // například změřit data po stisku tlačítka, abychom
  // tam neměli "starou" hodnotu. Nebo obecně resetovat
  // průběh.
  if (reset)
  { 
    state = READ_DATA;
    digitalWrite(LED_BUILTIN, LOW);
  }
 
  switch (state)
  {
    case READ_DATA:   // přečte data
      getMeasurements();
      timeLastTransition = millis();
      state = WAIT_DELAY;
      break;
    case WAIT_DELAY:   // čeká, než se data ze senzorů načtou znovu
      if (millis() - timeLastTransition >= READ_DELAY)
        state = READ_DATA;
      break;
    default:
      state = READ_DATA;
      break;
  }
}
