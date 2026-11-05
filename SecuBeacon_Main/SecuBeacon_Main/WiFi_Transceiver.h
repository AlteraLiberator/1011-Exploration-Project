
#ifndef WIFI_TRANSCEIVER_H
#define WIFI_TRANSCEIVER_H

#include <Arduino.h>
#include <WiFi101.h>

// Wi-Fi credentials
#define SECUBEACON_WIFI_SSID "Shine"
#define SECUBEACON_WIFI_PASSWORD "shine123"

// Python Telegram bridge
// Replace with your computer's actual IPv4 address
#define SECUBEACON_BRIDGE_IP "172.20.10.5"
#define SECUBEACON_BRIDGE_PORT 5000

class WiFiTransceiver {
private:
  WiFiClient client;
  bool connected;
  unsigned long lastAttempt;

public:
  WiFiTransceiver();

  void begin();
  void maintainConnection();
  bool sendAlert(const char* message);
  bool isConnected() const;
};

#endif
