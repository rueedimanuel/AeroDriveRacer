#pragma once
#include "Common.h"

struct SpeedOrientation
{
    float speed;
    float orientation; // Angle in degrees: 0 for straight, positive for right turn, negative for left turn
};

class Navigation
{
private:
    SpeedOrientation control;

public:
    Navigation() = delete;
    Navigation(float Speed, float Orientation);
    SpeedOrientation CalculateSpeedAndOrientation(float distanceLeft, float distanceRight, float distanceCenter);
    float GetOrientation();
    float GetSpeed();
};