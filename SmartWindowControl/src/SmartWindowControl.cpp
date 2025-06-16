#include "SmartWindowControl.h"

SmartWindowControl::SmartWindowControl() {
    client = new WiFiClientSecure;
    windowStateCurrent = LOW;
    servoIndex1 = -1;
}

void SmartWindowControl::begin() {
    Serial.begin(115200);
    delay(500);

    // Initialize servo
    ESP32_ISR_Servos.useTimer(3); // Use Timer 3
    servoIndex1 = ESP32_ISR_Servos.setupServo(SERVO_PIN, 544, 2450);
    if (servoIndex1 != -1)
        Serial.println(F("Setup Servo1 OK"));
    else
        Serial.println(F("Setup Servo1 failed"));

    pinMode(WATER_LEVEL_PIN, INPUT);
    pinMode(ONBOARD_LED_PIN, OUTPUT);
    digitalWrite(ONBOARD_LED_PIN, HIGH); // LED OFF

    connectToWiFi();
}

void SmartWindowControl::connectToWiFi() {
    WiFi.begin(SSID, PASSWORD);
    Serial.print("Connecting.");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        digitalWrite(ONBOARD_LED_PIN, LOW); delay(250);
        digitalWrite(ONBOARD_LED_PIN, HIGH); delay(250);
    }
    digitalWrite(ONBOARD_LED_PIN, LOW); // WiFi connected
    Serial.println("\nConnected to: " + String(SSID));
    Serial.println("IP address: " + WiFi.localIP().toString());
}

void SmartWindowControl::update() {
    water_level = analogRead(WATER_LEVEL_PIN);
    Serial.println("The water level value: " + String(water_level));

    if (client) {
        client->setCACert(root_cacert);
        if (https.begin(*client, server_url)) {
            rainsensor_data = httpGETRequest();
            JSONVar json_rainsensor_data = JSON.parse(rainsensor_data);
            if (JSON.typeof(json_rainsensor_data) == "undefined") {
                Serial.println("Parsing failed!");
                return;
            }

            rain_threshold = (int)(json_rainsensor_data["rain_threshold"]);
            rain_sensor_state = (const char*)(json_rainsensor_data["rain_sensor_state"]);
            window_state = (const char*)(json_rainsensor_data["window_state"]);

            Serial.println("Rain threshold: " + String(rain_threshold));
            Serial.println("Rain state: " + rain_sensor_state);
            Serial.println("Window state: " + window_state);

            controlWindow();
        } else {
            Serial.println("[HTTPS] Unable to connect");
        }
    }
}

void SmartWindowControl::controlWindow() {
    if (window_state == "1" || rain_sensor_state == "1") {
        if (windowStateCurrent == LOW) {
            for (position = 0; position <= 180; position += 10) {
                ESP32_ISR_Servos.setPosition(servoIndex1, position);
                delay(30);
            }
            windowStateCurrent = HIGH;
        }
    } else {
        if (windowStateCurrent == HIGH) {
            for (position = 180; position >= 0; position -= 10) {
                ESP32_ISR_Servos.setPosition(servoIndex1, position);
                delay(30);
            }
            windowStateCurrent = LOW;
        }
    }
}

String SmartWindowControl::httpGETRequest() {
    https.addHeader("Content-Type", "application/x-www-form-urlencoded");
    httpRequestData = "rain_sensor_API_KEY=" + String(API_KEY) + "&water_level=" + String(water_level);
    int httpResponseCode = https.POST(httpRequestData);

    Serial.print("Request: "); Serial.println(httpRequestData);
    Serial.print("HTTP Response code: "); Serial.println(httpResponseCode);

    if (httpResponseCode == 200) {
        payload = https.getString();
    } else {
        Serial.print("Error code: "); Serial.println(httpResponseCode);
    }
    https.end();
    return payload;
}
