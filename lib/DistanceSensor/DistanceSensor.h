#ifndef DistanceSensor_h
#define DistanceSensor_h

#include <Arduino.h>

class DistanceSensor
{
private:
    int echoPin;
    int triggerPin;
    float distance;

public:
    DistanceSensor(int echo, int trig);
    void updateDistance();
    float getDistance() const;
};

#endif
