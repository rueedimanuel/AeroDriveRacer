#pragma once
#include "Common.h"

struct SpeedOrientation
{
    float speed = 0;
    float orientation = 0; // Angle in degrees: 0 for straight, positive for right turn, negative for left turn
    float lastOrientations[10] = {0}; // Array to store last orientations
    float lastSpeed[30] = {0}; // Array to store last speed
    int orientationIndex = 0; 
    int SpeedIndex = 0;   
};

class Navigation
{
private:
    SpeedOrientation control;
    

public:
    SpeedOrientation CalculateSpeedAndOrientation(float distanceLeft, float distanceRight, float distanceCenter);
    float GetOrientation();
    float GetSpeed();
};