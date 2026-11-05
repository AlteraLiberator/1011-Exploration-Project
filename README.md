# SecuBeacon Asset Tracker - Setup & Deployment Guide

## 1. Hardware Stack Assembly
1. Stack the **TinyScreen+ (ASM2022)** at the base.
2. Snap on the **Accelerometer TinyShield (ASD2511-R-A)**.
3. Snap on the **WiFi TinyShield (ASD2123-R)**.
4. Snap on the **Wireling Adapter TinyShield (ASD2022)** and connect the **Buzzer Wireling (AST1035)** to Port A.
5. Connect the **150mAh Li-Poly Battery (ASR0003)** to the JST connector.

## 2. Software Architecture & Allocation
- **Member 1 (`SecuBeacon_Main.ino`):** Central coordinator loop, hardware pin mapping, and siren driving.
- **Member 2 (`SecuBeacon_Display.h / .cpp`):** OLED UI engine, state views, pin prompt graphics.
- **Member 3 (`Tamper_Detector.h / .cpp`):** BMA250 I2C accelerometer driver, delta-threshold motion filtering.
- **Member 4 (`WiFi_Transceiver.h / .cpp`):** WiFi101 library network client, TCP HTTP POST packet sender.
- **Member 5 (`telegram_bridge.py`):** Flask local HTTP listener, Telegram Bot API payload forwarder.
- **Member 6 (`Security_Engine.h / .cpp`):** Passcode buffer parser, security state machine transitions.

## 3. Deployment Steps
1. Open `telegram_bridge.py` and replace `TELEGRAM_BOT_TOKEN` and `TELEGRAM_CHAT_ID` with your Telegram credentials.
2. Run `python telegram_bridge.py` on your laptop. Note your laptop's local IP address (`ipconfig` / `ifconfig`).
3. Update `WiFi_Transceiver.h` with your local Wi-Fi SSID, password, and laptop IP address.
4. Compile and upload the C++ code to the TinyScreen+ via the Arduino IDE (selecting board **TinyScreen+**).
