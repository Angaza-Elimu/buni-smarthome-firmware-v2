#ifndef SMART_LIGHT_CONTROL_H
#define SMART_LIGHT_CONTROL_H

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <Arduino_JSON.h>
#include <driver/i2s.h>
#include "soc/i2s_reg.h"
#include "secrets.h"
#include "pin_configurations.h"

class SmartLightControl {
public:
    SmartLightControl();
    void begin();
    void update();

private:
    void initI2S();
    void initWiFi();
    void setColor(int r, int g, int b);
    String httpGETRequest(const char* url);

    int lightVal;
    int ldr_threshold;
    String ldr_status;
    float loudness;
};

#endif
