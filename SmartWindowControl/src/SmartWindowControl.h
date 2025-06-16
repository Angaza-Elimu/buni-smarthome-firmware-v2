#ifndef SMART_WINDOW_CONTROL_H
#define SMART_WINDOW_CONTROL_H

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include "ESP32_New_ISR_Servo.h"
#include "certificates.h"
#include "secrets.h"
#include "pin_configurations.h"

class SmartWindowControl {
public:
    SmartWindowControl();
    void begin();
    void update();

private:
    WiFiClientSecure *client;
    HTTPClient https;
    int water_level, threshold, position;
    int windowStateCurrent;
    int servoIndex1;
    String payload, httpRequestData;
    String rain_sensor_state;
    String window_state;
    String rainsensor_data;
    int rain_threshold;

    void connectToWiFi();
    void initServo();
    void controlWindow();
    String httpGETRequest();
};

#endif
