#include "Common.h"

#define TRIGGER_PIN 7    // Define the trigger pin
#define MAX_DISTANCE 200 // Maximum distance (in cm) to ping.
#define PROPELLERMOTOR_PIN 3
#define STERINGSERVO_PIN 5
#define MODESELECT_PIN 2

// Create instances of DistanceSensor
NewPing PingDistanceSensorL(TRIGGER_PIN, 8, MAX_DISTANCE); // Each sensor's trigger pin, echo pin, and max distance to ping.
NewPing PingDistanceSensorR(TRIGGER_PIN, 9, MAX_DISTANCE);
NewPing PingDistanceSensorC(TRIGGER_PIN, 10, MAX_DISTANCE);
unsigned long DistanceSensorL = 0;
unsigned long DistanceSensorR = 0;
unsigned long DistanceSensorC = 0;
Servo PropellerMotor;     // create servo object to control the Propeller Motor
Servo SteeringServo;      // create servo object to control the Steering Servo
Navigation AutoNav(0, 0); // instance of automatic Navigationcontrol
enum class SteeringMode
{
  off,
  manual,
  assisted
};
SteeringMode Mode;

void UpdateAllDistaceSensors();
void DEBUG();

void setup()
{
  Serial.begin(9600);
  PropellerMotor.attach(PROPELLERMOTOR_PIN);
  SteeringServo.attach(STERINGSERVO_PIN);
  Mode = SteeringMode::off;
  pinMode(MODESELECT_PIN, INPUT); // Steering modepin
}

void loop()
{
  // Set mode based on modeselect pin
  Mode = (digitalRead(MODESELECT_PIN) == HIGH) ? SteeringMode::manual : SteeringMode::off;

  // get all distances
  UpdateAllDistaceSensors();

  // calculate speed and orientation form distances
  AutoNav.CalculateSpeedAndOrientation(DistanceSensorL, DistanceSensorR, DistanceSensorC);

  // log
  DEBUG();

// control Motors
  switch (Mode)
  {
  case SteeringMode::off:
    PropellerMotor.write(10);
    break;

  case SteeringMode::manual:
    PropellerMotor.write(30);
    break;

  case SteeringMode::assisted:
    PropellerMotor.write(AutoNav.GetSpeed());
    SteeringServo.write(AutoNav.GetOrientation());
    break;

  default:
    break;
  }

  delay(500);
}

void DEBUG()
{
  Serial.print("Left Sensor: ");
  Serial.print(DistanceSensorL);
  Serial.print(" cm, Right Sensor: ");
  Serial.print(DistanceSensorR);
  Serial.print(" cm, Center Sensor: ");
  Serial.print(DistanceSensorC);
  Serial.println(" cm");

  Serial.print("Speed: ");
  Serial.print(AutoNav.GetSpeed());
  Serial.print(" / Orientation: ");
  Serial.println(AutoNav.GetOrientation()); // right is +, left is -
  Serial.print("Mode: ");
  switch (Mode)
  {
  case SteeringMode::off:
    Serial.println("Off");
    break;
  case SteeringMode::assisted:
    Serial.println("assisted");
    break;
  case SteeringMode::manual:
    Serial.println("manual");
    break;
  default:
    Serial.println("Unknown steering mode.");
  }
  Serial.println("----");
};

void UpdateAllDistaceSensors()
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
};