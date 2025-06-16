#include "IntrusionControl.h"
#include "certificates.h" // Only exposes extern declarations
#include <WiFi.h>
#include <HTTPClient.h>
#include "secrets.h"
#include "pin_configurations.h"

WiFiClientSecure client;
HTTPClient https;

void IntrusionControl::begin() {
    Serial.begin(115200);

    pinMode(PIR_PIN, INPUT);
    pinMode(ONBOARD_LED_PIN, OUTPUT);
    digitalWrite(ONBOARD_LED_PIN, HIGH);

    connectWiFi();

    client.setCACert(root_cacert); // From lib/certificates.a
}

void IntrusionControl::connectWiFi() {
    Serial.print("Connecting to WiFi");
    WiFi.begin(SSID, PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        digitalWrite(ONBOARD_LED_PIN, LOW);
        delay(250);
        digitalWrite(ONBOARD_LED_PIN, HIGH);
    }

    Serial.println();
    Serial.print("Connected! IP: ");
    Serial.println(WiFi.localIP());
}

void IntrusionControl::monitor() {
    int motion = digitalRead(PIR_PIN);

    if (motion == HIGH) {
        Serial.println("Motion detected!");
        sendAlert();
    } else {
        Serial.println("No motion.");
    }

    delay(3000); // Adjust detection interval
}

void IntrusionControl::sendAlert() {
    if (https.begin(client, server_url)) { // From lib/certificates.a
        https.addHeader("Content-Type", "application/x-www-form-urlencoded");
        String postData = "API_KEY=" + String(API_KEY) + "&alert=MotionDetected";

        int httpCode = https.POST(postData);
        if (httpCode > 0) {
            Serial.print("Alert sent. Server response: ");
            Serial.println(httpCode);
        } else {
            Serial.println("Failed to send alert.");
        }
        https.end();
    } else {
        Serial.println("HTTPS connection failed.");
    }
}
