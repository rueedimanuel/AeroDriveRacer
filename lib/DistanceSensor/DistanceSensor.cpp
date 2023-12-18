#include "DistanceSensor.h"

DistanceSensor::DistanceSensor(int echo, int trig) : echoPin(echo), triggerPin(trig), distance(0)
{
    pinMode(triggerPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

void DistanceSensor::updateDistance()
{
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);

    long duration = pulseIn(echoPin, HIGH, 11600); // Timeout for 2m at speed of sound
    distance = duration * 0.034 / 2;
}

float DistanceSensor::getDistance() const
{
    return distance;
}
