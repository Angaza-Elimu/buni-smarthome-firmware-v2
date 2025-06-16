#ifndef INTRUSION_CONTROL_H
#define INTRUSION_CONTROL_H

#include <Arduino.h>
#include <WiFiClientSecure.h>

class IntrusionControl {
public:
    void begin();
    void monitor();
private:
    void connectWiFi();
    void sendAlert();
};

#endif
