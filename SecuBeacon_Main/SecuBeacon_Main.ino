
#include <Wire.h>
#include <SPI.h>
#include <Wireling.h>

#include "SecuBeacon_Display.h"
#include "Tamper_Detector.h"
#include "WiFi_Transceiver.h"
#include "Security_Engine.h"

// ========================================
// HARDWARE CONFIGURATION
// ========================================

#define SECUBEACON_ENABLE_BUZZER 1
#define BUZZER_PIN A1
#define BUZZER_FREQUENCY 2700

// ========================================
// OBJECTS
// ========================================

SecuBeaconDisplay display;
TamperDetector tamper;
WiFiTransceiver wifi;
SecurityEngine engine;

// ========================================
// VARIABLES
// ========================================

bool alertPending = false;
unsigned long lastAlertAttempt = 0;

bool buzzerActive = false;
unsigned long lastDebug = 0;

// ========================================
// BUZZER CONTROL
// ========================================

void updateBuzzer(SystemState state) {

#if SECUBEACON_ENABLE_BUZZER

  bool shouldBuzz =
    (state == STATE_BREACH ||
     state == STATE_PASSCODE_ENTRY);

  if (shouldBuzz && !buzzerActive) {

    tone(BUZZER_PIN, BUZZER_FREQUENCY);
    buzzerActive = true;

    SerialUSB.println("BUZZER ON");
  }
  else if (!shouldBuzz && buzzerActive) {

    noTone(BUZZER_PIN);
    digitalWrite(BUZZER_PIN, LOW);

    buzzerActive = false;

    SerialUSB.println("BUZZER OFF");
  }

#endif
}

// ========================================
// SETUP
// ========================================

void setup() {

  SerialUSB.begin(115200);

  // Wait up to 10 seconds for Serial Monitor
  unsigned long start = millis();

  while (!SerialUSB && millis() - start < 10000UL) {
    delay(10);
  }

  SerialUSB.println("STEP 1: Setup started");

  // Initialize I2C
  Wire.begin();
  SerialUSB.println("STEP 2: Wire initialized");

  // Initialize Wireling Adapter
  Wireling.begin();
  SerialUSB.println("STEP 3: Wireling initialized");

#if SECUBEACON_ENABLE_BUZZER
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  SerialUSB.println("STEP 4: Buzzer initialized");
#endif

  // Initialize TinyScreen+
  SerialUSB.println("STEP 5: Starting display");
  display.begin();
  SerialUSB.println("STEP 6: Display initialized");

  // Initialize accelerometer
  SerialUSB.println("STEP 7: Starting accelerometer");

  bool sensorReady = tamper.begin();

  SerialUSB.println(
    sensorReady ?
    "STEP 8: Accelerometer OK" :
    "STEP 8: Accelerometer FAILED"
  );

  // Initialize security engine
  SerialUSB.println("STEP 9: Starting security engine");
  engine.begin();
  SerialUSB.println("STEP 10: Security engine initialized");

  // Show splash screen
  SerialUSB.println("STEP 11: Showing splash screen");
  display.showSplashScreen();
  delay(1000);
  SerialUSB.println("STEP 12: Splash screen complete");

  // Initialize WiFi
  // Uses the temporary non-blocking diagnostic version
  SerialUSB.println("STEP 13: Starting WiFi");
  wifi.begin();
  SerialUSB.println("STEP 14: WiFi initialized");

  SerialUSB.println("STEP 15: Setup complete");
}

// ========================================
// MAIN LOOP
// ========================================

void loop() {

  // Read TinyScreen buttons
  const uint8_t buttons = display.readButtons();

  // Read accelerometer
  const bool motion = tamper.checkTampering();

  // Maintain WiFi
  // Currently disabled inside WiFi_Transceiver.cpp
  wifi.maintainConnection();

  // Update security engine
  engine.update(buttons, motion);

  // Get security state
  const SystemState state = engine.getState();

  // ======================================
  // DEBUG OUTPUT EVERY SECOND
  // ======================================

  if (millis() - lastDebug >= 1000UL) {

    lastDebug = millis();

    SerialUSB.print("Buttons: ");
    SerialUSB.print(buttons);

    SerialUSB.print(" | Motion: ");
    SerialUSB.print(motion ? "YES" : "NO");

    SerialUSB.print(" | State: ");
    SerialUSB.print((int)state);

    SerialUSB.print(" | WiFi: ");
    SerialUSB.println(
      wifi.isConnected() ? "CONNECTED" : "DISCONNECTED"
    );
  }

  // ======================================
  // BREACH DETECTION
  // ======================================

  if (engine.isNewBreachEvent()) {

    alertPending = true;

    SerialUSB.println("SECURITY BREACH!");
  }

  // ======================================
  // BUZZER CONTROL
  // ======================================

  updateBuzzer(state);

  // ======================================
  // WIFI ALERT
  // ======================================

  if (
    alertPending &&
    wifi.isConnected() &&
    (unsigned long)(millis() - lastAlertAttempt)
      >= 5000UL
  ) {

    lastAlertAttempt = millis();

    SerialUSB.println("Sending security alert...");

    if (wifi.sendAlert(
      "SECUBEACON ALERT: Asset motion detected!"
    )) {

      alertPending = false;
      SerialUSB.println("Alert request sent");
    }
    else {

      SerialUSB.println("Alert request failed");
    }
  }

  // ======================================
  // UPDATE DISPLAY
  // ======================================

  display.renderState(
    state,
    engine.getEnteredPinLength(),
    wifi.isConnected()
  );

  delay(50);
}
