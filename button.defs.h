#define NOT_PRESSED HIGH
#define PRESSED LOW

typedef struct Buttons {
  const uint8_t pin = 0;
  const int debounce = 100;
  const unsigned long shortPress = 100;
  const unsigned long doublePress = 600;
  const unsigned long  longPress = 1000;

  unsigned long counter = 0;
  int shortPressAmount = 0;
  bool previousState = NOT_PRESSED;
  bool currentState;
} Button;