#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <DHT.h>

#define DHTPIN 22                  // DHT sensor pin
DHT dht(DHTPIN, DHT11);           // Create DHT11 sensor object

// Temperature and humidity characteristics (with Notify)
BLECharacteristic tempChar("cba1d466-344c-4be3-ab3f-189f80dd7518", BLECharacteristic::PROPERTY_NOTIFY);
BLECharacteristic humChar("ca73b3ba-39f6-4ab3-91ae-186dc9577d99", BLECharacteristic::PROPERTY_NOTIFY);

bool deviceConnected = false;     // Client connection status
unsigned long lastTime = 0;       // Last transmission time

// Callback class for client connect/disconnect
class ServerCB : public BLEServerCallbacks {
  void onConnect(BLEServer*) override { deviceConnected = true; }
  void onDisconnect(BLEServer*) override { deviceConnected = false; }
};

void setup() {
  Serial.begin(115200);
  dht.begin();                   // Initialize DHT sensor

  // Initialize BLE server
  BLEDevice::init("DHT_ESP32");
  auto server = BLEDevice::createServer();
  server->setCallbacks(new ServerCB());

  // Create service and add characteristics
  auto service = server->createService("91bad492-b950-4226-aa2b-4ede9fa42f59");
  service->addCharacteristic(&tempChar); tempChar.addDescriptor(new BLE2902());
  service->addCharacteristic(&humChar);  humChar.addDescriptor(new BLE2902());
  service->start();

  // Start advertising
  BLEDevice::getAdvertising()->addServiceUUID(service->getUUID());
  BLEDevice::getAdvertising()->start();
}

void loop() {
  if (deviceConnected && millis() - lastTime > 30000) {
    float t = dht.readTemperature(), h = dht.readHumidity();
    if (isnan(t) || isnan(h)) return;

    char buf[6];
    dtostrf(t, 6, 2, buf);
    tempChar.setValue(buf); tempChar.notify();
    dtostrf(h, 6, 2, buf);
    humChar.setValue(buf); humChar.notify();

    Serial.printf("Temperature: %.2f ºC - Humidity: %.2f %%\n", t, h);

    lastTime = millis();
  }
}

