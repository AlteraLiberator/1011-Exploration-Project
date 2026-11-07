#ifndef SECUBEACON_DISPLAY_H
#define SECUBEACON_DISPLAY_H

#include <TinyScreen.h>
#include "Security_Engine.h"

class SecuBeaconDisplay {
private:
  TinyScreen display;

public:
  SecuBeaconDisplay();
  void begin();
  uint8_t readButtons();
  void showSplashScreen();
  void renderState(SystemState state,
                   uint8_t pinLength,
                   bool wifiConnected);
  void displayBitmap(const uint8_t* bitmap, int x, int xLength, int y, int yLength);
};

#endif
