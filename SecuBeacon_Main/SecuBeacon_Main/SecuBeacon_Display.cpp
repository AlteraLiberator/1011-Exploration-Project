
#include "SecuBeacon_Display.h"
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

  display.clearScreen();

  display.drawLine(0, 0, 95, 0, CYBER_BLUE);
  display.drawLine(0, 63, 95, 63, CYBER_BLUE);

  display.setFont(liberationSans_10ptFontInfo);
  display.fontColor(CYBER_BLUE, CYBER_BLACK);
  display.setCursor(10, 18);
  display.print("SecuBeacon");

  display.setFont(liberationSans_8ptFontInfo);
  display.fontColor(CYBER_BLUE, CYBER_BLACK);
  display.setCursor(19, 39);
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

  display.setFont(liberationSans_8ptFontInfo);

  display.fontColor(CYBER_BLUE, CYBER_BLACK);
  display.setCursor(2, 1);
  display.print("SECU");

  display.fontColor(CYBER_BLUE, CYBER_BLACK);
  display.print("BEACON");

  display.fontColor(
    wifiConnected ? CYBER_BLUE : CYBER_RED,
    CYBER_BLACK
  );

  display.setCursor(72, 1);
  display.print(wifiConnected ? "ON" : "OFF");

  display.drawLine(0, 12, 95, 12, CYBER_BLUE);

  // ======================================
  // STATE SCREENS
  // ======================================

  switch (state) {

    // ====================================
    // DISARMED SCREEN
    // ====================================

    case STATE_DISARMED: {

      display.drawRect(
        3, 17, 90, 43, 0, CYBER_BLUE
      );

      display.setFont(liberationSans_10ptFontInfo);
      display.fontColor(CYBER_BLUE, CYBER_BLACK);
      display.setCursor(12, 24);
      display.print("DISARMED");

      display.setFont(liberationSans_8ptFontInfo);
      display.fontColor(CYBER_WHITE, CYBER_BLACK);
      display.setCursor(16, 44);
      display.print("B1 TO ARM");

      break;
    }

    // ====================================
    // ARMING SCREEN
    // ====================================

    case STATE_ARMING: {

      display.setFont(liberationSans_10ptFontInfo);
      display.fontColor(CYBER_BLUE, CYBER_BLACK);
      display.setCursor(15, 23);
      display.print("ARMING...");

      // Blue loading bar
      display.drawRect(
        13, 44, 70, 7, 0, CYBER_BLUE
      );

      display.drawRect(
        16, 46, 38, 3, 1, CYBER_BLUE
      );

      break;
    }

    // ====================================
    // FUTURISTIC CYBERLOCK ARMED SCREEN
    // ====================================

    case STATE_ARMED: {

      // Cyber HUD corners
      display.drawLine(3, 17, 15, 17, CYBER_BLUE);
      display.drawLine(3, 17, 3, 27, CYBER_BLUE);

      display.drawLine(80, 17, 92, 17, CYBER_BLUE);
      display.drawLine(92, 17, 92, 27, CYBER_BLUE);

      display.drawLine(3, 44, 3, 52, CYBER_BLUE);
      display.drawLine(3, 52, 15, 52, CYBER_BLUE);

      display.drawLine(92, 44, 92, 52, CYBER_BLUE);
      display.drawLine(80, 52, 92, 52, CYBER_BLUE);

      // Decorative lines
      display.drawLine(19, 30, 27, 30, CYBER_DIM);
      display.drawLine(19, 34, 25, 34, CYBER_BLUE);

      display.drawLine(69, 30, 77, 30, CYBER_DIM);
      display.drawLine(71, 34, 77, 34, CYBER_BLUE);

      // Lock shackle
      display.drawLine(36, 36, 36, 28, CYBER_BLUE);
      display.drawLine(36, 28, 41, 23, CYBER_BLUE);
      display.drawLine(41, 23, 54, 23, CYBER_BLUE);
      display.drawLine(54, 23, 59, 28, CYBER_BLUE);
      display.drawLine(59, 28, 59, 36, CYBER_BLUE);

      // Inner shackle
      display.drawLine(40, 36, 40, 29, CYBER_CYAN);
      display.drawLine(40, 29, 43, 27, CYBER_CYAN);
      display.drawLine(43, 27, 52, 27, CYBER_CYAN);
      display.drawLine(52, 27, 55, 29, CYBER_CYAN);
      display.drawLine(55, 29, 55, 36, CYBER_CYAN);

      // Lock body
      display.drawRect(
        31, 35, 33, 18, 1, CYBER_DIM
      );

      display.drawRect(
        33, 36, 29, 16, 0, CYBER_BLUE
      );

      display.drawRect(
        35, 38, 25, 12, 0, CYBER_CYAN
      );

      // Keyhole
      display.drawRect(
        46, 40, 4, 4, 1, CYBER_WHITE
      );

      display.drawLine(
        48, 44, 48, 47, CYBER_WHITE
      );

      // Status footer
      display.drawLine(
        0, 54, 95, 54, CYBER_BLUE
      );

      display.setFont(liberationSans_8ptFontInfo);

      display.fontColor(CYBER_BLUE, CYBER_BLACK);
      display.setCursor(2, 55);
      display.print("ARMED");

      display.fontColor(CYBER_BLUE, CYBER_BLACK);
      display.setCursor(53, 55);
      display.print("ACTIVE");

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
      display.setCursor(8, 20);
      display.print("! BREACH !");

      display.setFont(liberationSans_8ptFontInfo);
      display.fontColor(CYBER_WHITE, CYBER_BLACK);
      display.setCursor(14, 39);
      display.print("MOTION ALERT");

      display.fontColor(CYBER_BLUE, CYBER_BLACK);
      display.setCursor(16, 51);
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
      display.setCursor(15, 21);
      display.print("SECURITY PIN");

      display.setFont(liberationSans_10ptFontInfo);
      display.fontColor(CYBER_WHITE, CYBER_BLACK);
      display.setCursor(19, 40);

      for (uint8_t i = 0; i < 4; i++) {
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
