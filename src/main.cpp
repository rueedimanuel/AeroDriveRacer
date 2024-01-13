#include "Common.h"

//#define DEBUG 

#define TRIGGER_PIN 7             // Define the trigger pin for DistanceSensors
#define MAX_DISTANCE 250          // Maximum distance (in cm) to ping.
#define PROPELLERMOTOR_PIN 3
#define STERINGSERVO_PIN 5
#define STEERINGSERVO_MAX_R 48    // max range of SteeringServo to the right and left
#define STEERINGSERVO_MAX_L 127   

// Create instances of DistanceSensor
NewPing PingDistanceSensorL(TRIGGER_PIN, 8, MAX_DISTANCE); // Each sensor's trigger pin, echo pin, and max distance to ping.
NewPing PingDistanceSensorR(TRIGGER_PIN, 9, MAX_DISTANCE);
NewPing PingDistanceSensorC(TRIGGER_PIN, 10, MAX_DISTANCE);
unsigned long DistanceSensorL = 0;
unsigned long DistanceSensorR = 0;
unsigned long DistanceSensorC = 0;
Servo PropellerMotor;           // create servo object to control the Propeller Motor
Servo SteeringServo;            // create servo object to control the Steering Servo
Navigation AutoNav;       // instance of automatic Navigationcontrol
bool assistedSteering;          // if active use AutoNAv to help steering
bfs::SbusRx sbus_rx(&Serial);   // SBUS object, reading SBUS 
bfs::SbusData ReciverData;      // SBUS ReciverData

void UpdateAllDistaceSensors();
void MotroControl();
void log();


void setup()
{
  PropellerMotor.attach(PROPELLERMOTOR_PIN);
  SteeringServo.attach(STERINGSERVO_PIN);
  assistedSteering = false;

  // Initialize Serial Communication
  Serial.begin(100000);
  sbus_rx.Begin();

  // Initialize LED Pin
  pinMode(LED_BUILTIN, OUTPUT);
}


void loop(){

  // Get Sbus data
  if (sbus_rx.Read()) {
    ReciverData = sbus_rx.data();
  }

  assistedSteering = (ReciverData.ch[4] < 1024) ? false : true ;  // update assistedSteering based on signal from reciver
  digitalWrite(LED_BUILTIN, assistedSteering ? HIGH : LOW);       // set LED to on if assistedSteering is on

  // get all distances
  UpdateAllDistaceSensors();

  // calculate speed and orientation form distances
  AutoNav.CalculateSpeedAndOrientation(DistanceSensorL, DistanceSensorR, DistanceSensorC);

  #ifdef DEBUG
      log();
  #else
    // control Motors
    MotroControl();
  #endif


}



void log()
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

  delay(500);
}

void MotroControl(){
  if(assistedSteering){
    // control Steering
    if(ReciverData.ch[0] < 900 || 1100 < ReciverData.ch[0]){
      int SteeringOrientation = map(ReciverData.ch[0],172,1811,STEERINGSERVO_MAX_L,STEERINGSERVO_MAX_R); 
      SteeringServo.write(SteeringOrientation);
    }else{
    SteeringServo.write(map(AutoNav.GetOrientation(),-100,100,STEERINGSERVO_MAX_L,STEERINGSERVO_MAX_R));
    }

    // control PropellerSpeed
    float ManualPropellerSpeed = map(ReciverData.ch[2],172,1811,0,1000);
    ManualPropellerSpeed = ManualPropellerSpeed/1000; // to float
    float AutoNavPropellerSpeed = map(AutoNav.GetSpeed(),0,100,30,150);
    PropellerMotor.write(max(ManualPropellerSpeed * AutoNavPropellerSpeed,30));

  }else{
    // control Steering manually
    int SteeringOrientation = map(ReciverData.ch[0],172,1811,STEERINGSERVO_MAX_L,STEERINGSERVO_MAX_R); 
    SteeringServo.write(SteeringOrientation);
    // control PropellerSpeed
    int PropellerSpeed = map(ReciverData.ch[2],172,1811,30,150);
    PropellerMotor.write(PropellerSpeed);
  };
}

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
}