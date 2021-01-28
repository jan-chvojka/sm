#define NOT_PRESSED HIGH
#define PRESSED LOW

typedef struct Buttons {
  const uint8_t pin = D3;
  const int debounce = 50;
  const unsigned long shortPress = 50;
  const unsigned long doublePress = 600;
  const unsigned long  longPress = 500;

  unsigned long counter = 0;
  int shortPressAmount = 0;
  bool previousState = NOT_PRESSED;
  bool currentState;
} Button;