#include <WiFi.h>
#include <Adafruit_SSD1306.h>
#include "DHTControl.h"
#include "secrets.h"
#include "pin_configurations.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1      // OLED reset

// create display object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
WiFiClientSecure client;

// Instantiate DHTControl with display and fan motor pins
DHTControl dhtControl(DHT_PIN, DHT22, INA_PIN, INB_PIN, &display);

void setup() {
  Serial.begin(115200);

  // Initialize I2C with custom SDA and SCL
  Wire.begin(I2C_SDA, I2C_SCL);  // Important fix for custom I2C pins

  // validate OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED failed to initialize.");
    while (1);
  }

  // init the OLED display
  display.display();
  delay(2000);

  // set configs for ONBOARD_LED
  pinMode(ONBOARD_LED_PIN, OUTPUT);
  digitalWrite(ONBOARD_LED_PIN, HIGH);

  // initialize WiFi connection
  WiFi.begin(SSID, PASSWORD);

  // blink ONBOARD_LED on WiFiconnection
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(ONBOARD_LED_PIN, LOW);
    delay(250);
    digitalWrite(ONBOARD_LED_PIN, HIGH);
    delay(250);
  }

  // turn off led when connected
  digitalWrite(ONBOARD_LED_PIN, LOW);

  // initiate DHT
  dhtControl.begin();
}

// handle system Controls
void loop() {
  dhtControl.updateSensorReadings();
  dhtControl.sendDataAndControlFan(API_KEY, &client);
  delay(2000);
}
