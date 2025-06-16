#include "DHTControl.h"
#include "certificates.h"  // Include external certificate + URL source

DHTControl::DHTControl(uint8_t dhtPin, uint8_t dhtType, uint8_t inaPin, uint8_t inbPin, Adafruit_SSD1306* display)
  : _dht(dhtPin, dhtType), _inaPin(inaPin), _inbPin(inbPin), _display(display) {}

void DHTControl::begin() {
  _dht.begin();
  pinMode(_inaPin, OUTPUT);
  pinMode(_inbPin, OUTPUT);
}

// Get dht sensor readings momentarily
void DHTControl::updateSensorReadings() {
  _humidity = _dht.readHumidity();
  _temperature = _dht.readTemperature();
}

float DHTControl::getTemperature() {
  return _temperature;
}

float DHTControl::getHumidity() {
  return _humidity;
}

void DHTControl::sendDataAndControlFan(const char* apiKey, WiFiClientSecure* client) {
  // Use root CA certificate securely for HTTPS
  client->setCACert(root_cacert);

  /*
   Get POST request from the server
   Decode the payload
   */
  String response = httpPOSTRequest(apiKey, _temperature, _humidity, client);
  JSONVar json = JSON.parse(response);

  if (JSON.typeof(json) == "undefined") return;

  _tempThreshold = (int)json["temp_threshold"];
  _humidThreshold = (int)json["humid_threshold"];

  // Control fan based on server thresholds
  if (_temperature > _tempThreshold) {
    digitalWrite(_inaPin, LOW);
    digitalWrite(_inbPin, HIGH);
  } else {
    digitalWrite(_inaPin, LOW);
    digitalWrite(_inbPin, LOW);
  }

  // Update OLED display
  _display->clearDisplay();
  _display->setTextSize(1);
  _display->setCursor(0, 0);
  _display->print("Temperature: ");
  _display->setTextSize(2);
  _display->setCursor(0, 16);
  _display->print(_temperature);
  _display->print(" C");

  _display->setTextSize(1);
  _display->setCursor(0, 35);
  _display->print("Humidity: ");
  _display->setTextSize(2);
  _display->setCursor(0, 45);
  _display->print(_humidity);
  _display->print(" %");
  _display->display();
}

  /*
   Handle payload posting
   */
String DHTControl::httpPOSTRequest(const char* apiKey, float temp, float humi, WiFiClientSecure* client) {
  HTTPClient https;
  String payload;

  if (https.begin(*client, server_url)) {
    https.addHeader("Content-Type", "application/x-www-form-urlencoded");
    String data = "dht_sensor_API_KEY=" + String(apiKey) +
                  "&temperature=" + String(temp) +
                  "&humidity=" + String(humi);

    int httpCode = https.POST(data);
    if (httpCode == 200) {
      payload = https.getString();
    }
    https.end();
  }

  return payload;
}
