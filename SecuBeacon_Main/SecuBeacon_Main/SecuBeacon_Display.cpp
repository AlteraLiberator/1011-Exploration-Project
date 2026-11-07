
#include "SecuBeacon_Display.h"
#include "bitmaps.h"
#include <TinyScreen.h>

// ========================================
// CYBERLOCK 2.3 - BLUE THEME
// ========================================

#define CYBER_BLACK  TS_8b_Black
#define CYBER_BLUE   TS_8b_Blue
#define CYBER_CYAN   TS_8b_Blue
#define CYBER_DIM    TS_8b_Blue
#define CYBER_GREEN  TS_8b_Green
#define CYBER_RED    TS_8b_Red
#define CYBER_WHITE  TS_8b_White

// ========================================
// CONSTRUCTOR
// ========================================

SecuBeaconDisplay::SecuBeaconDisplay()
  : display(TinyScreenPlus) {}

// ========================================
// INITIALIZATION
// ========================================

void SecuBeaconDisplay::begin() {
  display.begin();
  display.setBrightness(10);
  display.setFlip(true);
}

uint8_t SecuBeaconDisplay::readButtons() {
  return display.getButtons();
}

// ========================================
// SPLASH SCREEN
// ========================================

void SecuBeaconDisplay::showSplashScreen() {
  displayBitmap(secuBeaconSplashScreen, 0, 96, 0, 64);

  display.setFont(liberationSans_8ptFontInfo);
  display.fontColor(CYBER_BLUE, CYBER_BLACK);
  display.setCursor(5, 63 - display.getFontHeight());
  display.print("CYBERLOCK 2.3");
}

// ========================================
// MAIN DISPLAY RENDERER
// ========================================

void SecuBeaconDisplay::renderState(
  SystemState state,
  uint8_t pinLength,
  bool wifiConnected
) {

  // ======================================
  // ANTI-FLICKER SYSTEM
  // ======================================

  static int lastState = -1;
  static int lastPinLength = -1;
  static int lastWifiStatus = -1;

  if (
    lastState == (int)state &&
    lastPinLength == (int)pinLength &&
    lastWifiStatus == (int)wifiConnected
  ) {
    return;
  }

  lastState = (int)state;
  lastPinLength = (int)pinLength;
  lastWifiStatus = (int)wifiConnected;

  // Redraw only when something changes
  display.clearScreen();

  // ======================================
  // HEADER
  // ======================================

  displayBitmap(headerBase, 0, 96, 0, 10);
  displayBitmap(wifiConnected ? wifi_Connected : wifi_Disconnected, 81, 11, 1, 8);

  // ======================================
  // STATE SCREENS
  // ======================================

  switch (state) {

    // ====================================
    // DISARMED SCREEN
    // ====================================

    case STATE_DISARMED: {
      displayBitmap(disarmedIconLarge, (96 - 21) / 2 - 1, 21, 15, 27);

      display.setFont(liberationSans_10ptFontInfo);
      display.fontColor(0xFF, 0x00);
      display.setCursor((96 - display.getPrintWidth("DISARMED")) / 2 - 1, 50);
      display.print("DISARMED");

      break;
    }

    // ====================================
    // ARMING SCREEN
    // ====================================

    case STATE_ARMING: {

      displayBitmap(disarmedIcon, 5, 7, 15, 9);

      display.setFont(liberationSans_8ptFontInfo);
      display.fontColor(0xFF, 0x00);
      display.setCursor(13, 15);
      display.print("Disarmed");

      display.setFont(liberationSans_10ptFontInfo);
      display.fontColor(0xFF, 0x00);
      display.setCursor((96 - display.getPrintWidth("Arming...")) / 2 - 1, 30);
      display.print("ARMING...");

      // Blue loading bar
      display.drawRect(13, 44, 70, 7, 0, CYBER_BLUE);
      display.drawRect(15, 46, 11, 3, 1, CYBER_BLUE);
      delay(500);
      display.drawRect(15, 46, 33, 3, 1, CYBER_BLUE);
      delay(800);
      display.drawRect(15, 46, 55, 3, 1, CYBER_BLUE);
      delay(1000);
      display.drawRect(15, 46, 66, 3, 1, CYBER_BLUE);
      delay(1000);
      break;
    }

    // ====================================
    // FUTURISTIC CYBERLOCK ARMED SCREEN
    // ====================================

    case STATE_ARMED: {

      displayBitmap(armedIcon, 5, 7, 15, 9);

      display.setFont(liberationSans_8ptFontInfo);
      display.fontColor(0xFF, 0x00);
      display.setCursor(13, 15);
      display.print("Armed");
      
      display.fontColor(0x03, 0x00);
      display.print(" (Active)");

      display.fontColor(0xFF, 0x00);
      display.setCursor(5, 15 + 5 + display.getFontHeight());
      display.print("Motion Detection");
      display.setCursor(5, 15 + 5 + 2*display.getFontHeight());
      display.print("Enabled");
      break;
    }

    // ====================================
    // BREACH ALERT SCREEN
    // ====================================

    case STATE_BREACH: {

      display.drawRect(
        2, 16, 92, 44, 0, CYBER_RED
      );

      display.setFont(liberationSans_10ptFontInfo);
      display.fontColor(CYBER_RED, CYBER_BLACK);
      display.setCursor((96 - display.getPrintWidth("! BREACH !")) / 2 - 1, 20);
      display.print("! BREACH !");

      display.setFont(liberationSans_8ptFontInfo);
      display.fontColor(CYBER_WHITE, CYBER_BLACK);
      display.setCursor((96 - display.getPrintWidth("MOTION ALERT")) / 2 - 1, 39);
      display.print("MOTION ALERT");

      display.fontColor(CYBER_BLUE, CYBER_BLACK);
      display.setCursor((96 - display.getPrintWidth("ENTER PIN")) / 2 - 1, 54);
      display.print("ENTER PIN");

      break;
    }

    // ====================================
    // PASSCODE ENTRY SCREEN
    // ====================================

    case STATE_PASSCODE_ENTRY: {

      display.drawRect(
        3, 17, 90, 43, 0, CYBER_BLUE
      );

      display.setFont(liberationSans_8ptFontInfo);
      display.fontColor(CYBER_BLUE, CYBER_BLACK);
      display.setCursor((96 - display.getPrintWidth("SECURITY PIN")) / 2 - 1, 21);
      display.print("SECURITY PIN");

      display.setFont(liberationSans_10ptFontInfo);
      display.fontColor(CYBER_WHITE, CYBER_BLACK);
      
      for (uint8_t i = 0; i < 4; i++) {
        display.setCursor((96 - 44) / 2 - 1 + i*11, 40);
        if (i < pinLength) {
          display.print("* ");
        } else {
          display.print("_ ");
        }
      }

      break;
    }

    default:
      break;
  }
}

// Call this method using the available bitmap arrays I created inside bitmap.cpp. You can add your own custom
void SecuBeaconDisplay::displayBitmap(const uint8_t* bitmap, int x, int xLength, int y, int yLength) {
  // Update Screen
  display.setX(x, x + xLength - 1);
  display.setY(y, y + yLength - 1);
  display.startData();
  display.writeBuffer((uint8_t*)bitmap, xLength * yLength);
  display.endTransfer();
}