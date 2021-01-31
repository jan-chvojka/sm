WiFiClient espClient;
PubSubClient client(espClient);

String clientId = "smsc01";
char mqttServer[] = "192.168.1.10";
char pubTopic[] = "sensors/test";     //payload[0] will control/set LED

void mqttFSM(void (*displayMessage)(char* text), bool connect = true) {

  static enum
  {
    CONNECT,
    CHECK_CONNECTION,
    RETRY_CONNECTION,
    CONNECTED,
    DISCONNECTED,
    OFFLINE
  } state = CONNECT;
  static uint32_t  lastRetryAttempt = 0;  
  static int previousMqttState = 100;
  
  int mqttState = client.state();
  bool connected = mqttState == 0;

  if(!connect)
    state = OFFLINE;

  char message[32];
  const uint32_t CONNECTION_RETRY_TIME = 5 * 1000;

  if(previousMqttState != mqttState) {
    // zobrazovat jen pokud je potřeba se připojit
    if(connect) {
      switch(connected) {
        case true: 
          snprintf(message, sizeof(message), "mqtt: connected");  
        break;
        case false: 
          snprintf(message, sizeof(message), "mqtt: error [%d]", mqttState);  
        break;
      }
    displayMessage(message);
    }
    
    previousMqttState = mqttState;
  }

  // if(previousMqttState != mqttState) {
  //   snprintf(message, sizeof(message), "mqtt: [%d]", mqttState);
  //   Serial.println(message);
  //   displayMessage(message);
  //   previousMqttState = mqttState;
  // }

  switch(state) {
    case CONNECT:
      Serial.println("CONNECT");
      client.setClient(espClient);
      client.setServer(mqttServer, 1883);
      client.connect(clientId.c_str());
      state = CHECK_CONNECTION;
    break;
    case CHECK_CONNECTION:
      if(connected)
        state = CONNECTED;
      else 
        state = DISCONNECTED;
    break;   
    case RETRY_CONNECTION:
      if (millis() - lastRetryAttempt >= CONNECTION_RETRY_TIME || connected) {
        state = CHECK_CONNECTION;
      }
    break;
    case CONNECTED:
      if(!connected)
        state = CHECK_CONNECTION;
    break;
    case DISCONNECTED:
      Serial.println("DISCONNECTED");
      lastRetryAttempt = millis();
      state = RETRY_CONNECTION;
    case OFFLINE:
      Serial.println("OFFLINE");
      if(connect)
        state = CONNECT;      
    break;
  }

}