
#ifndef TAMPER_DETECTOR_H
#define TAMPER_DETECTOR_H

#include <Arduino.h>
#include <Wire.h>

#define BMA250_I2C_ADDR 0x18
#define MOTION_THRESHOLD 45

class TamperDetector {
  int16_t lastX, lastY, lastZ;
  bool hasSample;

public:
  TamperDetector();
  bool begin();
  bool checkTampering();
};

#endif
