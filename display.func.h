// Deklarace SSD1306 displeje připojeného na I2C (SDA, SCL piny)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setupDisplay() {
  #if SERIAL
  Serial.println("[setupDisplay] begin");
  #endif
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    #if SERIAL
    Serial.println(F("[setupDisplay] SSD1306 allocation failed"));
    #endif
    for(;;);
  }  

  #if SERIAL
  Serial.println("[setupDisplay] end");
  #endif
}

void displayWelcomeMessage() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  // Display static text
  display.println("SMS/C");
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

void displayStatusMessage(String message) {
  display.setTextSize(1);
  deleteLine(56,63);
  
  display.setCursor(0, 56);
  display.println(message);

  display.display(); 
}

void displayStatusMessage(char* message) {
  char messageToDisplay[128];
  strncpy(messageToDisplay, message, 128);
  display.setTextSize(1);
  deleteLine(56,63);
  
  display.setCursor(0, 56);
  display.println(messageToDisplay);

  display.display(); 
}

/**
 * Zobrazí hodnoty na displeji.
 */
void displayLines(String line1, String line2) {
  deleteLine(0,15);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  // Display static text
  display.println(line1);

  deleteLine(16,32);
  display.setCursor(0, 16);
  display.println(line2);
  
  display.display(); 
}