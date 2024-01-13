#include "Common.h"


SpeedOrientation Navigation::CalculateSpeedAndOrientation(float distanceLeft, float distanceRight, float distanceCenter)
{

// Constants
    const float maxSpeed = 100;
    const float safeDistance = 50;
    const float criticalDistance = 30;

    // Default to full speed, straight ahead
    control.speed = maxSpeed;
    control.orientation = 0;

    // Calculate the closest distance among left, right, and center
    float closestDistance = min(min(distanceLeft, distanceRight), distanceCenter);

    // Adjust speed based on closest obstacle
    control.speed = min(map(closestDistance, criticalDistance, safeDistance, 0, maxSpeed),100);

    // Check for obstacles and adjust orientation
    if (closestDistance < safeDistance)
    {
        // Adjust orientation
        if (distanceLeft < safeDistance || distanceRight < safeDistance)
        {
            // Choose direction with more space
            float directionBias = (distanceRight - distanceLeft);

            // Clamp the orientation values
            control.orientation = max(-100.0f, min(100.0f, directionBias));
        }
        else if (distanceCenter < safeDistance)
        {
            // If center is blocked, choose the direction with more space
            control.orientation = (distanceLeft > distanceRight) ? -80 : 80;
        }
    }
    // Update the array with the new orientation
    control.lastOrientations[control.orientationIndex] = control.orientation;
    control.orientationIndex = (control.orientationIndex+1) % 10; // Move to the next index, wrapping around if necessary

    // Update the array with the new Speed
    control.lastSpeed[control.SpeedIndex] = control.speed;
    control.SpeedIndex = (control.SpeedIndex+1) % 30; // Move to the next index, wrapping around if necessary

    return control;
}

float Navigation::GetOrientation()
{
float sum = 0;
    for (int i = 0; i < 10; i++) {
        sum += control.lastOrientations[i];
    }
    return sum / 10;
};

float Navigation::GetSpeed()
{
    float sum = 0;
    for (int i = 0; i < 30; i++) {
        sum += control.lastSpeed[i];
    }
    return sum / 30;
};