#include "SecuBeacon_Display.h"
#include <TinyScreen.h>

SecuBeaconDisplay::SecuBeaconDisplay()
  : display(TinyScreenPlus) {}

void SecuBeaconDisplay::begin() {
  display.begin();
  display.setBrightness(10);  // Keep low to preserve battery
  display.setFlip(true);      // Adjust based on your enclosure orientation
}

uint8_t SecuBeaconDisplay::readButtons() {
  return display.getButtons();
}

void SecuBeaconDisplay::showSplashScreen() {
  display.clearScreen();

  // Draw top and bottom border accents
  display.drawLine(0, 0, 96, 0, TS_8b_Blue);
  display.drawLine(0, 63, 96, 63, TS_8b_Blue);

  // Main Title
  display.setFont(liberationSans_10ptFontInfo);
  display.fontColor(TS_8b_White, TS_8b_Black);
  display.setCursor(12, 18);
  display.print("SecuBeacon");

  // Subtitle / Version
  display.setFont(liberationSans_8ptFontInfo);
  display.fontColor(TS_8b_Gray, TS_8b_Black);
  display.setCursor(20, 35);
  display.print("v1.0 Ready");
}

void SecuBeaconDisplay::renderState(SystemState state, uint8_t pinLength, bool wifiConnected) {
  display.clearScreen(); 

  const uint8_t TS_8b_DarkBlue = 0x60;
  const uint8_t TS_8b_DarkGreen = 0x0C;

  uint8_t bgColor = TS_8b_Black;
  
  switch (state) {
    case STATE_DISARMED:
      bgColor = TS_8b_DarkBlue;
      break;
    case STATE_ARMED:
      bgColor = TS_8b_DarkGreen;
      break;
    case STATE_BREACH:
    case STATE_PASSCODE_ENTRY:
      bgColor = TS_8b_Red;
      break;
    default:
      bgColor = TS_8b_Black;
      break;
  }

  if (bgColor != TS_8b_Black) {
    display.drawRect(0, 13, 96, 51, 1, bgColor); 
  }

  // ==========================================
  // 1. STATUS BAR (Top 12 pixels - ALWAYS BLACK)
  // ==========================================
  display.setFont(liberationSans_8ptFontInfo);
  display.setCursor(2, 2);
  
  if (wifiConnected) {
    display.fontColor(TS_8b_Green, TS_8b_Black);
    display.print("WiFi: OK");
  } else {
    display.fontColor(TS_8b_Red, TS_8b_Black);
    display.print("WiFi: --");
  }

  display.drawLine(0, 12, 96, 12, TS_8b_Gray);

  // ==========================================
  // 2. MAIN CONTENT AREA
  // ==========================================
  
  switch (state) {
    case STATE_DISARMED:
      // Use a larger, naturally bolder font for the primary state text
      display.setFont(liberationSans_12ptFontInfo); 
      display.fontColor(TS_8b_White, bgColor);
      display.setCursor(8, 25); // Adjusted X cursor leftward to center the larger text
      display.print("DISARMED");

      // Switch back to smaller font for the subtext
      display.setFont(liberationSans_8ptFontInfo);
      display.fontColor(TS_8b_Yellow, bgColor); 
      display.setCursor(18, 48);
      display.print("[B1] to Arm");
      break;

    case STATE_ARMING:
      display.setFont(liberationSans_10ptFontInfo);
      display.fontColor(TS_8b_Yellow, bgColor);
      display.setCursor(16, 25);
      display.print("ARMING...");
      
      display.drawLine(16, 45, 80, 45, TS_8b_Yellow); 
      break;

    case STATE_ARMED:
      // Larger, bolder font
      display.setFont(liberationSans_12ptFontInfo);
      display.fontColor(TS_8b_Black, bgColor);
      display.setCursor(16, 25); // Adjusted X cursor to center
      display.print("ARMED");

      display.setFont(liberationSans_8ptFontInfo);
      display.fontColor(TS_8b_Black, bgColor);
      display.setCursor(15, 48);
      display.print("Monitoring...");
      break;

    case STATE_BREACH:
      // Larger, bolder font
      display.setFont(liberationSans_12ptFontInfo);
      display.fontColor(TS_8b_Black, bgColor);
      display.setCursor(5, 16); // Shifted left to fit the larger text
      display.print("! BREACH !");

      // Mid Prompt 
      display.setFont(liberationSans_8ptFontInfo);
      display.setCursor(18, 32);
      display.print("ENTER PIN:");

      // Bottom Blanks 
      display.setFont(liberationSans_10ptFontInfo);
      display.setCursor(18, 46);
      for (uint8_t i = 0; i < 4; i++) {
        display.print("_ "); 
      }
      break;

    case STATE_PASSCODE_ENTRY:
      display.fontColor(TS_8b_Black, bgColor);
      
      display.setFont(liberationSans_8ptFontInfo);
      display.setCursor(18, 32);
      display.print("ENTER PIN:");

      display.setFont(liberationSans_10ptFontInfo);
      display.setCursor(18, 46);
      for (uint8_t i = 0; i < 4; i++) {
        if (i < pinLength) {
          display.print("* "); 
        } else {
          display.print("_ "); 
        }
      }
      break;
  }
}