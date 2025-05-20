#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>

int scanTime = 5;
BLEScan* pBLEScan;
const String targetName = "202035349_Shim";

const int txPower = -59;
const float pathLossExponent = 2.0;
float lastDistance = -1.0;

const int ledPin = 21;  // GPIO21

float calculateDistance(int rssi) {
  return pow(10.0, ((float)(txPower - rssi)) / (10.0 * pathLossExponent));
}

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    if (advertisedDevice.getName() == targetName.c_str()) {
      int rssi = advertisedDevice.getRSSI();
      lastDistance = calculateDistance(rssi);

      Serial.printf("RSSI: %.0f dBm | Distance: %.2f m\n", (float)rssi, lastDistance);
    }
  }
};

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
}

void loop() {

  pBLEScan->start(scanTime, false);
  pBLEScan->clearResults();

  if (lastDistance > 0 && lastDistance <= 1.0) {
    digitalWrite(ledPin, HIGH);
    delay(800);
    digitalWrite(ledPin, LOW);
    delay(800);
  } else {
    digitalWrite(ledPin, LOW);
    delay(500);
  }
}