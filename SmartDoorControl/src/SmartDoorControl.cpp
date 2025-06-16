#include "SmartDoorControl.h"
#include "certificates.h"  // Secure server_url and root_cacert declarations

SmartDoorControl::SmartDoorControl(int relayPin, int buzzerPin) {
  _relayPin = relayPin;
  _buzzerPin = buzzerPin;
}

void SmartDoorControl::begin() {
  pinMode(_relayPin, OUTPUT);
  pinMode(_buzzerPin, OUTPUT);
  digitalWrite(_relayPin, LOW);   // Default: Locked
  digitalWrite(_buzzerPin, LOW);  // Default: Silent
}

void SmartDoorControl::lockDoor() {
  digitalWrite(_relayPin, LOW);  // Lock: LOW signal
}

void SmartDoorControl::unlockDoor() {
  digitalWrite(_relayPin, HIGH);  // Unlock: HIGH signal
}

void SmartDoorControl::soundBuzzer(unsigned long duration) {
  digitalWrite(_buzzerPin, HIGH);
  delay(duration);
  digitalWrite(_buzzerPin, LOW);
}

void SmartDoorControl::connectToServer() {
  Serial.print("Connecting to server: ");
  Serial.println(server_url); // From certificates.h (resolved via libcertificates.a)
}
