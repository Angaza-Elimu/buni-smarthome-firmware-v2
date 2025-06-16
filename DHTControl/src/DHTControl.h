#ifndef DHTCONTROL_H
#define DHTCONTROL_H

#include <DHT.h>
#include <WiFi.h>
#include <Arduino_JSON.h>
#include <Adafruit_SSD1306.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include "secrets.h"
#include "pin_configurations.h"

/*
 * This class handles reading temperature & humidity from a DHT sensor,
 * sending the data securely to a remote server, and controlling a fan
 * based on thresholds returned from the server.
 */
class DHTControl {
public:
  DHTControl(uint8_t dhtPin, uint8_t dhtType, uint8_t inaPin, uint8_t inbPin, Adafruit_SSD1306* display);
  void begin();
  void updateSensorReadings();

  // This function sends data securely using an internally stored CA cert and server URL.
  void sendDataAndControlFan(const char* apiKey, WiFiClientSecure* client);

  float getTemperature();
  float getHumidity();

private:
  DHT _dht;
  float _temperature;
  float _humidity;
  int _tempThreshold;
  int _humidThreshold;
  uint8_t _inaPin, _inbPin;
  Adafruit_SSD1306* _display;

  String httpPOSTRequest(const char* apiKey, float temp, float humi, WiFiClientSecure* client);
};

#endif
