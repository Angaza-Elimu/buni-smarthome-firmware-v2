#ifndef SMART_DOOR_CONTROL_H
#define SMART_DOOR_CONTROL_H

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <WiFi.h>
#include <Keypad.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFiClientSecure.h>
#include <ESP32_New_ISR_Servo.h>
#include "secrets.h"
#include "pin_configurations.h"

class SmartDoorControl {
  public:
    SmartDoorControl(int relayPin, int buzzerPin);

    void begin();
    void lockDoor();
    void unlockDoor();
    void soundBuzzer(unsigned long duration);
    void connectToServer();     // Demonstrates secure server connection logic

  private:
    int _relayPin;
    int _buzzerPin;
};

#endif
