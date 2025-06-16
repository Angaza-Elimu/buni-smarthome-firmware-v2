#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <IntrusionControl.h>
#include "secrets.h"
#include "pin_configurations.h"

IntrusionControl intrusion;

void setup() {
  intrusion.begin();
}

void loop() {
  intrusion.monitor();
}
