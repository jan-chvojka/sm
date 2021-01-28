// Deklarace tlačítka
Button button;

void (*handleShortPressCallback)();

void handleShortPress() {
  #ifdef SERIAL
  Serial.println("short Press");
  # endif
  handleShortPressCallback();
}

void handleDoublePress() {
  #ifdef SERIAL
  Serial.println("Double Press");
  # endif
}

void handleLongPress() {
  #ifdef SERIAL
  Serial.println("Long Press");
  # endif
}

void setupButton(void (*handleShortPressFn)()) {
  handleShortPressCallback = handleShortPressFn;
  pinMode(button.pin, INPUT_PULLUP);
}

void loopButton() {
  button.currentState = digitalRead(button.pin);

  if (button.currentState != button.previousState) {
    delay(button.debounce);
    // update status in case of bounce
    button.currentState = digitalRead(button.pin);
    if (button.currentState == PRESSED) {
      button.counter = millis();
    }
    else if (button.currentState == NOT_PRESSED) {
      unsigned long currentMillis = millis();

      if ((currentMillis - button.counter >= button.shortPress) && !(currentMillis - button.counter >= button.longPress)) {
        // short press detected, add press to amount
        button.shortPressAmount++;
      }
      else if ((currentMillis - button.counter >= button.longPress)) {
        // long press was detected
        handleLongPress();
      }
    }
    button.previousState = button.currentState;
  }
  
  if (button.shortPressAmount >= 2) {
    button.shortPressAmount = 0;
    handleDoublePress();
  }
  else if (button.shortPressAmount == 1) {
    unsigned long currentMillis = millis();
    if (currentMillis - button.counter >= button.doublePress) {
      handleShortPress();
      button.shortPressAmount = 0;
    }
  }
}