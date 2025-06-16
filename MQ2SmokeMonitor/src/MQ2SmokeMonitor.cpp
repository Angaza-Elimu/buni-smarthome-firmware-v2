#include "MQ2SmokeMonitor.h"
#include "certificates.h"

#define TONE_PWM_CHANNEL 0

int gasValue = 0;
int smoke_threshold = 0;
String httpRequestData, payload;
HTTPClient https;
WiFiClientSecure* client = new WiFiClientSecure;

void MQ2SmokeMonitor::begin() {
    Serial.begin(115200);
    delay(10);

    pinMode(ONBOARD_LED_PIN, OUTPUT);
    digitalWrite(ONBOARD_LED_PIN, HIGH); // LED off initially
    pinMode(MQ2_PIN, INPUT);

    ledcAttachPin(BUZZER_PIN, TONE_PWM_CHANNEL);

    connectWiFi();

    // Attach certificate to secure client
    client->setCACert(root_cacert);
}

void MQ2SmokeMonitor::connectWiFi() {
    Serial.println("\n\nConnecting to WiFi...");
    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        digitalWrite(ONBOARD_LED_PIN, LOW);
        delay(250);
        digitalWrite(ONBOARD_LED_PIN, HIGH);
    }

    digitalWrite(ONBOARD_LED_PIN, LOW);
    Serial.println("\nConnected to WiFi.");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
}

void MQ2SmokeMonitor::checkSmokeLevel() {
    gasValue = analogRead(MQ2_PIN);
    Serial.println("\nSmoke sensor value: " + String(gasValue));

    if (!client) {
        Serial.println("[HTTPS] Secure client error");
        return;
    }

    Serial.println("[HTTPS] Begin communication...");

    if (https.begin(*client, server_url)) {
        String smokesensor_data = httpGETRequest(server_url);
        JSONVar json_threshold = JSON.parse(smokesensor_data);

        if (JSON.typeof(json_threshold) == "undefined") {
            Serial.println("Failed to parse server response.");
            return;
        }

        smoke_threshold = (int) json_threshold["smoke_threshold"];
        Serial.println("Server-set threshold: " + String(smoke_threshold));

        if (gasValue > smoke_threshold) {
            ledcWriteNote(TONE_PWM_CHANNEL, NOTE_C, 4);
            delay(500);
            ledcWriteTone(TONE_PWM_CHANNEL, 800);
            delay(500);
        }

    } else {
        Serial.println("[HTTPS] Connection failed.");
    }

    delay(3000); // Delay between checks
}

String MQ2SmokeMonitor::httpGETRequest(const char* serverName) {
    https.addHeader("Content-Type", "application/x-www-form-urlencoded");

    httpRequestData = "smoke_sensor_API_KEY=" + String(API_KEY) +
                      "&smoke_sensor_val=" + String(gasValue);

    int httpResponseCode = https.POST(httpRequestData);

    Serial.println("POST data: " + httpRequestData);

    if (httpResponseCode == 200) {
        payload = https.getString();
        Serial.println("Response: " + payload);
    } else {
        Serial.print("HTTP Error: ");
        Serial.println(httpResponseCode);
    }

    https.end();
    return payload;
}
