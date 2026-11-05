
#include "Tamper_Detector.h"

TamperDetector::TamperDetector()
  : lastX(0), lastY(0), lastZ(0), hasSample(false) {}

bool TamperDetector::begin() {
  Wire.beginTransmission(BMA250_I2C_ADDR);
  Wire.write((uint8_t)0x0F);
  Wire.write((uint8_t)0x03);
  return Wire.endTransmission() == 0;
}

bool TamperDetector::checkTampering() {
  Wire.beginTransmission(BMA250_I2C_ADDR);
  Wire.write((uint8_t)0x02);

  if (Wire.endTransmission(false) != 0)
    return false;

  if (Wire.requestFrom((uint8_t)BMA250_I2C_ADDR,
                       (uint8_t)6) != (uint8_t)6)
    return false;

  int16_t rawX = (uint16_t)Wire.read();
  rawX |= (uint16_t)Wire.read() << 8;

  int16_t rawY = (uint16_t)Wire.read();
  rawY |= (uint16_t)Wire.read() << 8;

  int16_t rawZ = (uint16_t)Wire.read();
  rawZ |= (uint16_t)Wire.read() << 8;

  int16_t x = rawX >> 6;
  int16_t y = rawY >> 6;
  int16_t z = rawZ >> 6;

  if (!hasSample) {
    lastX = x;
    lastY = y;
    lastZ = z;
    hasSample = true;
    return false;
  }

  bool motion =
    abs(x - lastX) > MOTION_THRESHOLD ||
    abs(y - lastY) > MOTION_THRESHOLD ||
    abs(z - lastZ) > MOTION_THRESHOLD;

  lastX = x;
  lastY = y;
  lastZ = z;

  return motion;
}
