#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <MQ2SmokeMonitor.h>
#include "secrets.h"
#include "pin_configurations.h"

MQ2SmokeMonitor smoke;

void setup() {
  smoke.begin();
}

void loop() {
  smoke.checkSmokeLevel();
}
