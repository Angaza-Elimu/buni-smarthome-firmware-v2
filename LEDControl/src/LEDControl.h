#ifndef LEDCONTROL_H
#define LEDCONTROL_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "secrets.h"
#include "pin_configurations.h"

class LEDControl {
  public:
    LEDControl(int ledPin, const char* ssid, const char* password, const char* apiKey);
    void begin();
    void updateLEDState();

  private:
    int _ledPin;
    const char* _ssid;
    const char* _password;
    const char* _apiKey;

    WiFiClientSecure _client;
    HTTPClient _https;
    String httpGETRequest(const char* serverName);

    String ledState;
    int blinkInterval;
    int blinkIntensity;
};

#endif
