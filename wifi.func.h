// #if DEBUG
//   FSM_STATE(s) { Serial.print(F("n:")); Serial.print(F(s)); }
//   FSM_ERROR(s) { Serial.print(F("n:")); Serial.print(F(s)); }
// #else
//   FSM_STATE(s) {}
//   FSM_ERROR(s) {}
// #endif

int WifiConnectionAttempt = 0;
unsigned long timeLastWifiConnectionAttempt = 0;

void wifiConnect() {
  WiFi.disconnect();
  #if SERIAL
  Serial.println("[wifiFSM::CONNECT] begin");
  Serial.println("[wifiFSM::CONNECT] ssid: " + String(WIFI_SSID));
  Serial.println("[wifiFSM::CONNECT] pass: " + String(WIFI_PASSWORD));
  # endif
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  // WiFi.setmode(STATION);
}

bool wifiFSM(void (*displayMessage)(char* text)) {

  static enum  
  {
    CONNECT,
    CONNECTION_DELAY,
    CHECK_CONNECTION,
    CONNECTED,
    DISCONNECTED
  } state = CONNECT;

  const uint32_t CONNECTION_DELAY_TIME = 20 * 1000;
  static uint32_t  lastConnectAttempt = 0;
  char message[32];

  static bool connected = false;
  static int previousStatus = 100;
  wl_status_t currentStatus = WiFi.status();

  if(previousStatus != currentStatus) {
    switch(currentStatus)
    {
      case WL_CONNECTED:
          snprintf(message, sizeof(message), "WiFi: [%s]", WiFi.localIP().toString().c_str());  
          break;
      default:
          snprintf(message, sizeof(message), "WiFi: disconnected [%d]", currentStatus);  
        break;
    }
    displayMessage(message);
    previousStatus = currentStatus;
  }

  switch (state)
  {
    case CONNECT:
      wifiConnect();
      lastConnectAttempt = millis();
      state = CONNECTION_DELAY;
      break;    
    case CONNECTION_DELAY:
      if (millis() - lastConnectAttempt >= CONNECTION_DELAY_TIME || currentStatus == WL_CONNECTED)
        state = CHECK_CONNECTION;
      break;
    case CHECK_CONNECTION:
      if(currentStatus == WL_CONNECTED)
        state = CONNECTED;
      else {
        state = DISCONNECTED;
      }
      break;  
    case CONNECTED:
      connected = true;
      if(currentStatus != WL_CONNECTED) {
        state = CHECK_CONNECTION;
      }
      break;
    case DISCONNECTED:
      connected = false;
      lastConnectAttempt = millis();
      state = CONNECTION_DELAY;
      break;      
  }

  return connected;

}