int WifiConnectionAttempt = 0;
unsigned long timeLastWifiConnectionAttempt = 0;

void setupWifi(void (*displayMessage)(String text)) {
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
    displayMessage(String(message));
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

// void displayWiFiStatus() {
//   #if WIFI
//     return;
//   #endif

//   deleteLine(48,63);
//   display.setTextSize(1);
//   display.setCursor(0, 48);
//   if(WiFi.status() == WL_CONNECTED) 
//   {
//     display.println("WiFi: OK");
//   }
//   else 
//   {
//     display.println("WiFi: Error");
//   }
//   display.display(); 
// }

void displayWiFiStatus(void (*displayMessage)(String text)) {
  #if !WIFI
    return;
  #endif
  
  if(WiFi.status() == WL_CONNECTED) 
  {
    displayMessage("WiFi: OK");
  }
  else 
  {
    displayMessage("WiFi: Error");
  }
}