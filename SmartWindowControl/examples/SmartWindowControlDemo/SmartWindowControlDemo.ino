#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include "ESP32_New_ISR_Servo.h"
#include <SmartWindowControl.h>
#include "secrets.h"
#include "pin_configurations.h"

// Instantiate SmartWindowControl
SmartWindowControl smartWindow;

void setup() {
  smartWindow.begin();
}

void loop() {
  smartWindow.update();
  delay(3000);
}
