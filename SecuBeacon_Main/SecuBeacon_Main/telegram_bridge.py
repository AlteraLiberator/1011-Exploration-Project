import time
import threading
import requests
from flask import Flask, request

app = Flask(__name__)

# --- Telegram Bot Configuration ---
TELEGRAM_BOT_TOKEN = "9055971956:AAHotvLJ4FbP0VcauYiKxey9KZEvKbtSKfY"
TELEGRAM_CHAT_ID   = "824143605"

# TinyScreen+ hardware IP address
TINYSCREEN_IP = "172.20.10.3"

# =================================================
# 1. FLASK ROUTE: HARDWARE ALERT -> TELEGRAM
# =================================================
@app.route('/alert', methods=['POST'])
def receive_alert():
    data = request.get_json()
    if data and 'alert' in data:
        message = data['alert']
        print(f"\n[!!!] ALERT RECEIVED: {message}", flush=True)
        
        telegram_url = f"https://api.telegram.org/bot{TELEGRAM_BOT_TOKEN}/sendMessage"
        payload = {
            "chat_id": TELEGRAM_CHAT_ID,
            "text": f"🚨 SecuBeacon Alert: {message}"
        }
        
        try:
            response = requests.post(telegram_url, json=payload, timeout=5)
            print(f"[Telegram API Response]: {response.status_code} - {response.text}", flush=True)
        except requests.exceptions.RequestException as e:
            print(f"[!] Failed to reach Telegram API: {e}", flush=True)
            
        return "Alert processed", 200
    return "Invalid data", 400

# =================================================
# 2. BACKGROUND THREAD: TELEGRAM -> HARDWARE BUZZER
# =================================================
def telegram_command_poller():
    print("[*] Telegram command listener running in background...", flush=True)
    last_update_id = 0
    
    while True:
        try:
            url = f"https://api.telegram.org/bot{TELEGRAM_BOT_TOKEN}/getUpdates"
            params = {"offset": last_update_id + 1, "timeout": 30}
            
            response = requests.get(url, params=params, timeout=35)
            
            if response.status_code == 200:
                data = response.json()
                for update in data.get("result", []):
                    last_update_id = update["update_id"]
                    
                    msg = update.get("message", {})
                    text = msg.get("text", "").strip()
                    chat_id = str(msg.get("chat", {}).get("id", ""))
                    
                    if chat_id == TELEGRAM_CHAT_ID and text.startswith("/"):
                        print(f"\n[Telegram Command Received]: {text}", flush=True)
                        
                        if text == "/buzzer_on":
                            try:
                                r = requests.get(f"http://{TINYSCREEN_IP}/buzzer/on", timeout=3)
                                print(f"[Hardware Response]: {r.status_code} - Buzzer ON sent to TinyScreen+", flush=True)
                            except requests.exceptions.RequestException as e:
                                print(f"[!] Failed to reach hardware at {TINYSCREEN_IP}: {e}", flush=True)
                                
                        elif text == "/buzzer_off":
                            try:
                                r = requests.get(f"http://{TINYSCREEN_IP}/buzzer/off", timeout=3)
                                print(f"[Hardware Response]: {r.status_code} - Buzzer OFF sent to TinyScreen+", flush=True)
                            except requests.exceptions.RequestException as e:
                                print(f"[!] Failed to reach hardware at {TINYSCREEN_IP}: {e}", flush=True)
                                
        except requests.exceptions.RequestException:
            # Silent catch for normal network blips/timeouts during long polling
            time.sleep(3)
        except Exception as e:
            print(f"[!] Polling error: {e}", flush=True)
            time.sleep(5)
            
        time.sleep(1)

# =================================================
# MAIN ENTRY POINT
# =================================================
if __name__ == '__main__':
    poller_thread = threading.Thread(target=telegram_command_poller, daemon=True)
    poller_thread.start()
    
    app.run(host='0.0.0.0', port=5000, debug=False, use_reloader=False)