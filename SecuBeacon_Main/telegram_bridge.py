from flask import Flask, request
import requests

app = Flask(__name__)

# --- Telegram Bot Configuration ---
TELEGRAM_BOT_TOKEN = "9055971956:AAHotvLJ4FBp0VcauYiKxey9KZEVKbtSKfY"
TELEGRAM_CHAT_ID   = "824143605"

@app.route('/alert', methods=['POST'])
def receive_alert():
    data = request.get_json()
    if data and 'alert' in data:
        message = data['alert']
        print(f"[!!!] ALERT RECEIVED: {message}")
        
        # Forward to Telegram
        telegram_url = f"https://api.telegram.org/bot{TELEGRAM_BOT_TOKEN}/sendMessage"
        payload = {
            "chat_id": TELEGRAM_CHAT_ID,
            "text": f" SecuBeacon Alert: {message}"
        }
        requests.post(telegram_url, json=payload)
        return "Alert processed", 200
    return "Invalid data", 400

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)