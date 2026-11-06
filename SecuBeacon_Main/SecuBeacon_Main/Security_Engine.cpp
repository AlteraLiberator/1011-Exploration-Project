
#include "Security_Engine.h"

SecurityEngine::SecurityEngine()
  : state(STATE_DISARMED),
    inputIndex(0),
    previousButtons(0),
    newBreachFlag(false),
    armTimer(0) {
  targetPin[0] = 1;
  targetPin[1] = 2;
  targetPin[2] = 1;
  targetPin[3] = 2;
}

void SecurityEngine::begin() {
  state = STATE_DISARMED;
  inputIndex = 0;
  previousButtons = 0;
  newBreachFlag = false;
}

void SecurityEngine::update(uint8_t buttons,
                            bool motionDetected) {
  newBreachFlag = false;

  const uint8_t pressed = buttons & ~previousButtons;
  previousButtons = buttons;

  switch (state) {
    case STATE_DISARMED:
      if (pressed & 0x01) {
        state = STATE_ARMING;
        armTimer = millis();
      }
      break;

    case STATE_ARMING:
      if ((unsigned long)(millis() - armTimer) >= 3000UL)
        state = STATE_ARMED;
      break;

    case STATE_ARMED:
      if (motionDetected) {
        state = STATE_BREACH;
        newBreachFlag = true;
      }
      break;

    case STATE_BREACH:
      if (pressed & 0x03) {
        state = STATE_PASSCODE_ENTRY;
        inputIndex = 0;
        currentInput[inputIndex++] =
          (pressed & 0x01) ? 1 : 2;
      }
      break;

    case STATE_PASSCODE_ENTRY:
      if ((pressed & 0x03) && inputIndex < 4) {
        currentInput[inputIndex++] =
          (pressed & 0x01) ? 1 : 2;
      }

      if (inputIndex == 4) {
        bool match = true;

        for (uint8_t i = 0; i < 4; i++) {
          if (currentInput[i] != targetPin[i])
            match = false;
        }

        state = match ? STATE_DISARMED : STATE_BREACH;
        inputIndex = 0;
      }
      break;
  }
}

SystemState SecurityEngine::getState() const {
  return state;
}

uint8_t SecurityEngine::getEnteredPinLength() const {
  return inputIndex;
}

bool SecurityEngine::isNewBreachEvent() const {
  return newBreachFlag;
}
