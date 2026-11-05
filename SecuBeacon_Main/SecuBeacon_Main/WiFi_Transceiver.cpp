
#include "WiFi_Transceiver.h"
#include <string.h>

WiFiTransceiver::WiFiTransceiver()
  : connected(false), lastAttempt(0) {}

// ========================================
// INITIALIZE WIFI
// ========================================

void WiFiTransceiver::begin() {

  // Correct TinyCircuits ASD2123-R pin mapping
  WiFi.setPins(8, 2, A3, -1);

  SerialUSB.println("WiFi pins configured");

  lastAttempt = millis() - 15000UL;

  maintainConnection();
}

// ========================================
// MAINTAIN WIFI CONNECTION
// ========================================

void WiFiTransceiver::maintainConnection() {

  connected = (WiFi.status() == WL_CONNECTED);

  if (connected) {
    return;
  }

  if ((unsigned long)(millis() - lastAttempt)
      < 15000UL) {
    return;
  }

  lastAttempt = millis();

  SerialUSB.println("Connecting to WiFi...");

  WiFi.begin(
    SECUBEACON_WIFI_SSID,
    SECUBEACON_WIFI_PASSWORD
  );

  connected = (WiFi.status() == WL_CONNECTED);

  if (connected) {
    SerialUSB.println("WiFi CONNECTED!");

    IPAddress ip = WiFi.localIP();

    SerialUSB.print("IP Address: ");
    SerialUSB.print(ip[0]);
    SerialUSB.print(".");
    SerialUSB.print(ip[1]);
    SerialUSB.print(".");
    SerialUSB.print(ip[2]);
    SerialUSB.print(".");
    SerialUSB.println(ip[3]);
  }
  else {
    SerialUSB.println("WiFi connection failed");
  }
}

// ========================================
// SEND TELEGRAM BRIDGE ALERT
// ========================================

bool WiFiTransceiver::sendAlert(const char* message) {

  if (WiFi.status() != WL_CONNECTED) {
    connected = false;
    return false;
  }

  SerialUSB.println("Connecting to Telegram bridge...");

  if (!client.connect(
      SECUBEACON_BRIDGE_IP,
      SECUBEACON_BRIDGE_PORT)) {

    SerialUSB.println("Bridge connection failed");
    return false;
  }

  const size_t bodyLength = strlen(message) + 12;

  client.print(F("POST /alert HTTP/1.1\r\n"));
  client.print(F("Host: "));
  client.print(SECUBEACON_BRIDGE_IP);
  client.print(F("\r\nContent-Type: application/json\r\n"));
  client.print(F("Connection: close\r\n"));
  client.print(F("Content-Length: "));
  client.print(bodyLength);
  client.print(F("\r\n\r\n"));

  client.print(F("{\"alert\":\""));
  client.print(message);
  client.print(F("\"}"));

  client.flush();
  delay(100);
  client.stop();

  SerialUSB.println("Alert request sent to bridge");

  return true;
}

// ========================================
// WIFI CONNECTION STATUS
// ========================================

bool WiFiTransceiver::isConnected() const {
  return connected;
}
