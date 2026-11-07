#ifndef BITMAP_H
#define BITMAP_H

#include <TinyScreen.h>

#define MAX_X 96
#define MAX_Y 64
#define MAX_PIXEL MAX_X*MAX_Y

// Bitmap Constants Here:

// SplashScreen
extern const uint8_t secuBeaconSplashScreen[MAX_PIXEL];

// Bitmaps for Header
extern const uint8_t headerBase[96*10];
extern const uint8_t wifi_Connected[11*8];
extern const uint8_t wifi_Disconnected[11*8];

// Bitmaps for Body
extern const uint8_t armedIcon[7*9];
extern const uint8_t disarmedIcon[7*9];
extern const uint8_t disarmedIconLarge[21*27];


#endif