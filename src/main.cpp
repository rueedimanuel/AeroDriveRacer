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
bool assistedSteering;

SoftwareSerial SerialELRS(12,13);

void UpdateAllDistaceSensors();
void DEBUG();

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  SerialELRS.begin(11000);
  Serial.begin(9600);
  PropellerMotor.attach(PROPELLERMOTOR_PIN);
  SteeringServo.attach(STERINGSERVO_PIN);
  assistedSteering = false;
  pinMode(MODESELECT_PIN, INPUT); // Steering modepin
}

void loop()
{
  SerialELRS.listen();
  Serial.println("Data from port one:");
  // while there is data coming in, read it
  // and send to the hardware serial port:
  while (SerialELRS.available() > 0) {
    char inByte = SerialELRS.read();
    Serial.write(inByte);
  }

  // get all distances
  UpdateAllDistaceSensors();

  // calculate speed and orientation form distances
  AutoNav.CalculateSpeedAndOrientation(DistanceSensorL, DistanceSensorR, DistanceSensorC);

  // log
  DEBUG();

// control Motors
  if(assistedSteering){
    PropellerMotor.write(AutoNav.GetSpeed());
    SteeringServo.write(AutoNav.GetOrientation());
  }else{
    PropellerMotor.write(30);
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
  if(assistedSteering){
    Serial.println("assisted");
  }else{
    Serial.println("manual");
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