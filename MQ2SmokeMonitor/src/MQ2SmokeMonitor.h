#ifndef MQ2_SMOKE_MONITOR_H
#define MQ2_SMOKE_MONITOR_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include "secrets.h"
#include "pin_configurations.h"

class MQ2SmokeMonitor {
public:
    void begin();
    void checkSmokeLevel();
private:
    void connectWiFi();
    String httpGETRequest(const char* serverName);
};

#endif
