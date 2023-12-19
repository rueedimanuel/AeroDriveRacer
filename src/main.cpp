#include "Common.h"

#define TRIGGER_PIN 13   // Define the trigger pin
#define MAX_DISTANCE 200 // Maximum distance (in cm) to ping.

// Create instances of DistanceSensor
NewPing PingDistanceSensorL(TRIGGER_PIN, 10, MAX_DISTANCE); // Each sensor's trigger pin, echo pin, and max distance to ping.
NewPing PingDistanceSensorR(TRIGGER_PIN, 11, MAX_DISTANCE);
NewPing PingDistanceSensorC(TRIGGER_PIN, 12, MAX_DISTANCE);
unsigned long DistanceSensorL = 0;
unsigned long DistanceSensorR = 0;
unsigned long DistanceSensorC = 0;
SpeedOrientation AutoNavControl{0, 0}; // speed and Orientation form auto navigation
Navigation AutoNav(0, 0);

void setup()
{
  Serial.begin(9600);
}

void loop()
{

  DistanceSensorL = PingDistanceSensorL.ping_cm();
  if (DistanceSensorL == 0)
  {
    DistanceSensorL = MAX_DISTANCE;
  };
  delay(10);
  DistanceSensorR = PingDistanceSensorR.ping_cm();
  if (DistanceSensorR == 0)
  {
    DistanceSensorR = MAX_DISTANCE;
  };
  delay(10);
  DistanceSensorC = PingDistanceSensorC.ping_cm();
  if (DistanceSensorC == 0)
  {
    DistanceSensorC = MAX_DISTANCE;
  };

  Serial.print("Left Sensor: ");
  Serial.print(DistanceSensorL);
  Serial.print(" cm, Right Sensor: ");
  Serial.print(DistanceSensorR);
  Serial.print(" cm, Center Sensor: ");
  Serial.print(DistanceSensorC);
  Serial.println(" cm");

  AutoNavControl = AutoNav.getSpeedAndOrientation(DistanceSensorL, DistanceSensorR, DistanceSensorC);

  Serial.print("speed: ");
  Serial.print(AutoNavControl.speed);
  Serial.print("orientation: ");
  Serial.println(AutoNavControl.orientation);

  delay(1000);
}
