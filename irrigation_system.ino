#include <DHT.h>

#define DHTPIN 4          // DHT22 data pin connected to GPIO 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

const int moisturePin = 34;   // Potentiometer (simulated soil moisture sensor) on GPIO 34
const int pumpRelayPin = 5;   // Relay (pump) on GPIO 5

const int dryThreshold = 2000;  // Below this value the soil is considered dry
const float maxTemp = 35.0;     // Maximum temperature for watering (°C)

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(pumpRelayPin, OUTPUT);
  digitalWrite(pumpRelayPin, LOW);  // Pump is off at startup
  Serial.println("Irrigation system started...");
}

void loop() {
  float t = dht.readTemperature();
  int moistureValue = analogRead(moisturePin);  // Reads a value from 0 to 4095

  if (isnan(t)) {
    Serial.println("Error: failed to read from DHT sensor!");
    digitalWrite(pumpRelayPin, LOW);  // Safety: turn the pump off
    delay(3000);                      // Wait before trying again
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" °C | Soil moisture: ");
  Serial.println(moistureValue);

  // Watering logic: if the soil is dry and the temperature is acceptable, turn on the water
  if (moistureValue < dryThreshold && t < maxTemp) {
    digitalWrite(pumpRelayPin, HIGH);  // Turn the pump on
    Serial.println("=> Soil is dry and temperature is fine -> Pump ON");
  } else {
    digitalWrite(pumpRelayPin, LOW);   // Turn the pump off
    Serial.println("=> Soil is wet or too hot -> Pump OFF");
  }

  delay(3000);  // Wait 3 seconds before reading again
}
