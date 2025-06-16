#include "certificates.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <Arduino_JSON.h>
#include <driver/i2s.h>
#include "soc/i2s_reg.h"
#include <SmartLightControl.h>
#include "pin_configurations.h"
#include "secrets.h"

// instantiate the controller
SmartLightControl controller;

void setup() {
  controller.begin();
}

void loop() {
  controller.update();
  delay(1000);
}
