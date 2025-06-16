#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <WiFiClientSecure.h>
#include <RGBLEDControl.h>
#include "certificates.h"
#include "secrets.h"

// Instantiate RGBLEDControl
RGBLEDControl rgbLED;

void setup() {
  rgbLED.begin();
}

void loop() {
  rgbLED.handleLEDState();
}
