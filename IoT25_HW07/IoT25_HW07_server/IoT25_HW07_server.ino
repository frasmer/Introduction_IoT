
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

void setup() {
  Serial.begin(115200);
  BLEDevice::init("202035349_Shim");
  BLEServer *pServer = BLEDevice::createServer();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->start();
  Serial.println("BLE Server advertising started...");
}

void loop() {
  delay(1000);
}
