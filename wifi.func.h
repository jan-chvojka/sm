// #if DEBUG
//   FSM_STATE(s) { Serial.print(F("n:")); Serial.print(F(s)); }
//   FSM_ERROR(s) { Serial.print(F("n:")); Serial.print(F(s)); }
// #else
//   FSM_STATE(s) {}
//   FSM_ERROR(s) {}
// #endif

int WifiConnectionAttempt = 0;
unsigned long timeLastWifiConnectionAttempt = 0;



void setupWifi(void (*displayMessage)(String text)) {
  WiFi.disconnect();
  delay(500);

  timeLastWifiConnectionAttempt = millis()/1000; 

  #if SERIAL
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
    displayMessage(String(message));
    delay(WIFI_CONNECTION_INTERVAL);
    
    if(WifiConnectionAttempt >= WIFI_SETUP_CONNECTION_ATTEMPTS
      && WiFi.status() != WL_CONNECTED) {
      #ifdef SERIAL
      Serial.println("[setupWifi] could not connect to wifi.");
      # endif
      break;  
    }
    #ifdef SERIAL
    Serial.print(".");
    # endif
  }
  #ifdef SERIAL

  Serial.println();
  WifiConnectionAttempt = 0;
  Serial.println("[setupWifi] end");
  # endif
}




void displayWiFiStatus(void (*displayMessage)(String text)) {
  #if !WIFI
    return;
  #endif

  static wl_status_t PrevStatus = WL_DISCONNECTED;
  wl_status_t CurrentStatus = WiFi.status();

  if(PrevStatus != CurrentStatus) {
    PrevStatus = CurrentStatus;
    if(CurrentStatus == WL_CONNECTED) 
    {
      displayMessage("WiFi: Connected");  
    }
    else 
    {
      displayMessage("WiFi: Error");
    }  
  }
}

void wifiConnect() {
  #if SERIAL
  Serial.println("[wifiFSM::CONNECT] begin");
  Serial.println("[wifiFSM::CONNECT] ssid: " + String(WIFI_SSID));
  Serial.println("[wifiFSM::CONNECT] pass: " + String(WIFI_PASSWORD));
  # endif
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
}

bool wifiFSM(void (*displayMessage)(char* text)) {
  static enum  
  {
    CONNECT,
    CONNECTION_DELAY,
    CHECK_CONNECTION,
    RECONNECT
  } state = CONNECT;

  const uint32_t CONNECTION_DELAY_TIME = 20 * 1000;
  const int CONNECTION_DELAY_ATTEMPTS = 3;
  uint32_t  lastConnectAttempt = 0;
  static int connectionAttempt = 0;
  char message[128];
  static bool previousConnected = false;

  switch (state)
  {
    case CONNECT:
      // FSM_STATE("[wifiFSM::CONNECT]");
      wifiConnect();
      connectionAttempt++;
      previousConnected = false;
      snprintf(message, sizeof(message), "WiFi: connecting [%i]", connectionAttempt);
      displayMessage(message);
      lastConnectAttempt = millis();
      state = CONNECTION_DELAY;
      break;
    case CONNECTION_DELAY:   // čeká, než ověříme, zda jsme připojeni
      if (millis() - lastConnectAttempt >= CONNECTION_DELAY_TIME || WiFi.status() == WL_CONNECTED)
        // FSM_STATE("[wifiFSM::CONNECTION_DELAY]");
        state = CHECK_CONNECTION;
      break;  
    case CHECK_CONNECTION:
      if(WiFi.status() == WL_CONNECTED) {
        if(!previousConnected) {
          snprintf(message, sizeof(message), "WiFi: connected");  
          displayMessage(message);
          previousConnected = true;
        }
        connectionAttempt = 0;
        state = CHECK_CONNECTION;
      }
      else {
        connectionAttempt++;
        snprintf(message, sizeof(message), "WiFi: connecting [%i]", connectionAttempt);
        displayMessage(message);
        if(connectionAttempt <= CONNECTION_DELAY_ATTEMPTS)  {
          lastConnectAttempt = millis();
          state = CONNECTION_DELAY;
        }
        else {
          state = RECONNECT;
        }
      }
      break;          
    case RECONNECT:
      // FSM_STATE("[wifiFSM::RECONNECT]");
      previousConnected = false;
      connectionAttempt = 0;
      snprintf(message, sizeof(message), "WiFi: reconnecting");
      displayMessage(message);
      WiFi.reconnect();
      lastConnectAttempt = millis();
      state = CONNECTION_DELAY;
      break;      
    default:
      state = CONNECT;
      break;
  }

  return WiFi.status() == WL_CONNECTED;
}


