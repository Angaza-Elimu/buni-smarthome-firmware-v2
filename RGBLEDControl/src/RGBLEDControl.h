#ifndef RGBLEDCONTROL_H
#define RGBLEDCONTROL_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <WiFiClientSecure.h>
#include "secrets.h"
#include "pin_configurations.h"

// Pin configurations
#define RED_PIN 15
#define GREEN_PIN 2
#define BLUE_PIN 4
#define ONBOARD_LED_PIN 5

class RGBLEDControl {
  public:
    RGBLEDControl();
    void begin();
    void handleLEDState();

  private:
    WiFiClientSecure* client;
    HTTPClient https;
    String httpGETRequest(const char* serverName);
    void RGB_Color(int red, int green, int blue);
};

#endif
