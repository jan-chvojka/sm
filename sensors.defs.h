// Knihovna pro senzor teploty
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

typedef struct HTSensors {
  float temperature = 0;
  float humidity = 0;
} HTSensor;