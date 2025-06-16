#include "LEDControl.h"
#include "certificates.h"  // Include external certificate + URL source

LEDControl::LEDControl(int ledPin, const char* ssid, const char* password, const char* apiKey)
  : _ledPin(ledPin), _ssid(ssid), _password(password), _apiKey(apiKey) {}

void LEDControl::begin() {
  pinMode(_ledPin, OUTPUT);
  digitalWrite(_ledPin, HIGH);

  Serial.print("\nConnecting to ");
  Serial.println(_ssid);
  WiFi.begin(_ssid, _password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    digitalWrite(_ledPin, LOW);
    delay(250);
    digitalWrite(_ledPin, HIGH);
    delay(250);
  }

  digitalWrite(_ledPin, LOW);
  Serial.println("\nConnected. IP: " + WiFi.localIP().toString());
}

void LEDControl::updateLEDState() {
  _client.setCACert(root_cacert);

  if (_https.begin(_client, server_url)) {
    String response = httpGETRequest(server_url);
    JSONVar json = JSON.parse(response);

    if (JSON.typeof(json) == "undefined") {
      Serial.println("JSON Parsing failed!");
      return;
    }

    ledState = (const char*)(json["led_control_state"]);
    blinkInterval = (int)(json["led_blink_interval"]);
    blinkIntensity = (int)(json["led_blink_intensity"]);

    Serial.println("LED blink interval: " + String(blinkInterval));
    Serial.println("LED blink intensity: " + String(blinkIntensity));

    if (ledState == "1") {
      Serial.println("LED status: ON");
      digitalWrite(_ledPin, HIGH);
      if (blinkInterval > 0) {
        digitalWrite(_ledPin, HIGH);
        delay(blinkInterval);
        digitalWrite(_ledPin, LOW);
        delay(blinkInterval);
      }
    } else {
      Serial.println("LED status: OFF");
      digitalWrite(_ledPin, LOW);
    }

    _https.end();
  } else {
    Serial.println("[HTTPS] Connection failed");
  }
}

String LEDControl::httpGETRequest(const char* serverName) {
  _https.addHeader("Content-Type", "application/x-www-form-urlencoded");
  String postData = "ledctrl_API_KEY=" + String(_apiKey);
  int responseCode = _https.POST(postData);

  Serial.println("Sending request: " + postData);

  if (responseCode == 200) {
    Serial.println("Response code: 200 OK");
    return _https.getString();
  } else {
    Serial.print("Error code: ");
    Serial.println(responseCode);
    return "";
  }
}
