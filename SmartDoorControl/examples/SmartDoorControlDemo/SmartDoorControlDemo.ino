//----------------------------------------Support libraries and sensor parameters.
#include "secrets.h"
#include "Tone32.hpp"
#include "pin_configurations.h"
#include <SmartDoorControl.h>

// Relay on pin 5, Buzzer on pin 4
SmartDoorControl door(5, 4);

void setup() {
  Serial.begin(115200);
  door.begin();

  door.connectToServer();    // Secure URL access

  door.unlockDoor();
  delay(2000);
  door.lockDoor();
  door.soundBuzzer(1000);
}

void loop() {
  // Optional continuous loop logic
}
