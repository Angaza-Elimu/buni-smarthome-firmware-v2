#include "RGBLEDControl.h"
#include "certificates.h"  // Include external certificate + URL source

#define R_channel 0
#define G_channel 1
#define B_channel 2
#define pwm_Frequency 5000
#define pwm_resolution 8

RGBLEDControl::RGBLEDControl() {
  client = new WiFiClientSecure;
}

void RGBLEDControl::begin() {
  Serial.begin(115200);

  ledcAttachPin(RED_PIN, R_channel);
  ledcAttachPin(GREEN_PIN, G_channel);
  ledcAttachPin(BLUE_PIN, B_channel);
  ledcSetup(R_channel, pwm_Frequency, pwm_resolution);
  ledcSetup(G_channel, pwm_Frequency, pwm_resolution);
  ledcSetup(B_channel, pwm_Frequency, pwm_resolution);

  WiFi.begin(SSID, PASSWORD);

  pinMode(ONBOARD_LED_PIN, OUTPUT);
  digitalWrite(ONBOARD_LED_PIN, HIGH);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    digitalWrite(ONBOARD_LED_PIN, LOW);
    delay(250);
    digitalWrite(ONBOARD_LED_PIN, HIGH);
    delay(250);
  }

  digitalWrite(ONBOARD_LED_PIN, LOW);
  Serial.println("\nConnected to WiFi: " + String(SSID));
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  client->setCACert(root_cacert);
}

/*
  RGBLED control handler
  Control RGBLED remotely
*/
void RGBLEDControl::handleLEDState() {
  if (https.begin(*client, server_url)) {
    String response = httpGETRequest(server_url);
    JSONVar data = JSON.parse(response);

    if (JSON.typeof(data) == "undefined") {
      Serial.println("Failed to parse JSON");
      return;
    }

    // decode received data
    int blink_interval = (int)(data["blink_interval"]);
    String toggle_state = (const char*)data["toggle_state"];
    int r = (int)(data["rled_slider_val"]);
    int g = (int)(data["gled_slider_val"]);
    int b = (int)(data["bled_slider_val"]);

    Serial.println("Blink: " + String(blink_interval));
    Serial.println("Toggle: " + toggle_state);
    Serial.println("RGB: " + String(r) + "," + String(g) + "," + String(b));

    // Control RGBLED
    if (toggle_state == "1") {
      if (blink_interval > 0) {
        RGB_Color(r, g, b);
        delay(blink_interval);
        RGB_Color(0, 0, 0);
        delay(blink_interval);
      } else {
        RGB_Color(r, g, b);
      }
    } else {
      RGB_Color(0, 0, 0);
    }
  } else {
    Serial.println("[HTTPS] Connection failed");
  }

  if (https.connected()) https.end();
  delay(3000);
}

// HPPT POST request handler
String RGBLEDControl::httpGETRequest(const char* serverName) {
  https.addHeader("Content-Type", "application/x-www-form-urlencoded");
  String data = "rgbled_toggle_API_KEY=" + String(API_KEY);
  int code = https.POST(data);

  if (code > 0) {
    Serial.println("HTTP Code: " + String(code));
    return https.getString();
  } else {
    Serial.println("Error Code: " + String(code));
    return "";
  }
}

// RGBLED control handler
void RGBLEDControl::RGB_Color(int red, int green, int blue) {
  ledcWrite(R_channel, red);
  ledcWrite(G_channel, green);
  ledcWrite(B_channel, blue);
}
