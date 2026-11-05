#ifndef SECURITY_ENGINE_H
#define SECURITY_ENGINE_H

#include <Arduino.h>

enum SystemState {
  STATE_DISARMED,
  STATE_ARMING,
  STATE_ARMED,
  STATE_BREACH,
  STATE_PASSCODE_ENTRY
};

class SecurityEngine {
  SystemState state;
  uint8_t targetPin[4];
  uint8_t currentInput[4];
  uint8_t inputIndex;
  uint8_t previousButtons;
  bool newBreachFlag;
  unsigned long armTimer;

public:
  SecurityEngine();
  void begin();
  void update(uint8_t buttons, bool motionDetected);
  SystemState getState() const;
  uint8_t getEnteredPinLength() const;
  bool isNewBreachEvent() const;
};

#endif
