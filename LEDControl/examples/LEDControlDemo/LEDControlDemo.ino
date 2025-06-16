#include <WiFi.h>
#include <LEDControl.h>
#include "secrets.h" // defines SSID, PASSWORD, API_KEY
#include "pin_configurations.h"

LEDControl smartLED(LED_PIN, SSID, PASSWORD, API_KEY); // API key provisioned at creation

void setup() {
  Serial.begin(115200);
  smartLED.begin();
}

void loop() {
  smartLED.updateLEDState();
  delay(3000);
}
