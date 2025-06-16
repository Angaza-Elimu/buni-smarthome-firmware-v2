#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>
#include <ESP32Servo.h>

// ==== OLED Configuration ====
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define I2C_SCL 2     // SCL pin for OLED
#define I2C_SDA 42    // SDA pin for OLED
#define SERVO_PIN 20  // Servo motor pin

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ==== Servo Motor ====
Servo doorServo;

// ==== Keypad Configuration ====
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {1, 10, 9, 11};         // ESP32-S2 Row GPIOs
byte colPins[COLS] = {12, 14, 21, 34};       // ESP32-S2 Col GPIOs

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// ==== Password Configuration ====
const String presetPassword = "1234";
String enteredPassword = "";

// ==== Setup ====
void setup() {
  Serial.begin(9600);
  Serial.println("System Initializing...");

  // Initialize I2C with custom SDA and SCL
  Wire.begin(I2C_SDA, I2C_SCL);
  Serial.println("I2C initialized");

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED initialization failed"));
    while (true); // Halt
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Smart Home");
  display.display();
  Serial.println("OLED display initialized");

  // Initialize Servo
  doorServo.attach(SERVO_PIN);
  doorServo.write(0);  // Locked position
  Serial.println("Servo attached and locked at 0 degrees");

  Serial.println("System Ready");
}

// ==== Main Loop ====
void loop() {
  char key = keypad.getKey();

  if (key) {
    Serial.print("Key Pressed: ");
    Serial.println(key);

    if (key == '*') {
      enteredPassword = "";
      Serial.println("Input cleared");
      showMessage("Cleared", 1000);
    } else if (key == '#') {
      Serial.print("Entered Password: ");
      Serial.println(enteredPassword);

      if (enteredPassword == presetPassword) {
        Serial.println("Password correct. Access granted.");
        showMessage("Access Granted", 1000);
        unlockDoor();
      } else {
        Serial.println("Incorrect password. Access denied.");
        showMessage("Password Error", 1500);
      }
      enteredPassword = ""; // Reset for next attempt
    } else {
      enteredPassword += key;
      Serial.print("Password so far: ");
      Serial.println(enteredPassword);
      showMessage("Password: " + enteredPassword, 500);
    }
  }
}

// ==== Helper Functions ====

// Display a message on OLED
void showMessage(String msg, unsigned long duration) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Smart Home");
  display.setCursor(0, 20);
  display.println(msg);
  display.display();

  Serial.print("OLED Display: ");
  Serial.println(msg);

  delay(duration);
}

// Unlock and lock the door using servo motor
void unlockDoor() {
  Serial.println("Unlocking door...");
  doorServo.write(90);  // Rotate to unlock
  delay(5000);          // Stay unlocked for 5 seconds

  Serial.println("Locking door...");
  doorServo.write(0);   // Rotate back to lock
  showMessage("Door Locked", 1000);
}
