#include "Arduino.h"
#include "DistanceSensor.h"

// Define the trigger pin
#define TRIGGER_PIN 13

// Create instances of DistanceSensor
DistanceSensor DistanceSensorL(10, TRIGGER_PIN); // Left sensor
DistanceSensor DistanceSensorR(11, TRIGGER_PIN); // Right sensor
DistanceSensor DistanceSensorC(12, TRIGGER_PIN); // Center sensor

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  DistanceSensorL.updateDistance();
  DistanceSensorR.updateDistance();
  DistanceSensorC.updateDistance();

  Serial.print("Left Sensor: ");
  Serial.print(DistanceSensorL.getDistance());
  Serial.print(" cm, Right Sensor: ");
  Serial.print(DistanceSensorR.getDistance());
  Serial.print(" cm, Center Sensor: ");
  Serial.print(DistanceSensorC.getDistance());
  Serial.println(" cm");

  delay(10); // Minimal delay to avoid serial buffer overflow
}
