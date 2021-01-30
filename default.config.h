///////////////////////////////////////////////////////////////////////////////
// SERIAL
// Pokud chcete použít sériový výstup a sledovat
// dění v programu pomocí serial monitoru, nastavte
// true. 
//
// DEBUG
// Pro ladění jsou nastaveny nižší intervaly,
// aby byla funkčnost lépe vidět.
///////////////////////////////////////////////////////////////////////////////
#define SERIAL true
#define DEBUG true

///////////////////////////////////////////////////////////////////////////////
// Jen při použití WiFi.
///////////////////////////////////////////////////////////////////////////////
#define WIFI true

// WiFi config
#define WIFI_CONNECTION_INTERVAL 5 * 1000
#define WIFI_RECONNECTION_INTERVAL 30 * 1000
#define WIFI_SETUP_CONNECTION_ATTEMPTS 3

#define WIFI_SSID "WIFI_SSID"
#define WIFI_PASSWORD "WIFI_PASSWORD"
#define DEVICE_SECRET_KEY "DEVICE_SECRET_KEY"

#define SERVER_ADDRESS "SERVER_ADDRESS"
#define SERVER_PATH "PATH"

#define  MQTT_SERVER "MQTT_SERVER"
#define  MQTT_USERNAME "MQTT_USERNAME"
#define  MQTT_PASSWORD "MQTT_PASSWORD"

///////////////////////////////////////////////////////////////////////////////
// Hodnoty pro program
///////////////////////////////////////////////////////////////////////////////

#if SERIAL
#define MEASUREMENT_INTERVAL 5 * 1000
#else
#define MEASUREMENT_INTERVAL 20 * 1000
#endif

// Vlastnosti displeje
#define SCREEN_WIDTH 128 // šířka displeje v px
#define SCREEN_HEIGHT 64 // výška displeje v px

// Nastavení adresy senzoru
#define BME280_ADRESA (0x76)

#define DISPLAY_ON_DURATION 30 * 1000
