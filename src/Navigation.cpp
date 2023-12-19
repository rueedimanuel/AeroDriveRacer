#include "Common.h"

Navigation::Navigation(float Speed, float Orientation)
    : control{Speed, Orientation} {}; // right + left -

SpeedOrientation Navigation::getSpeedAndOrientation(float distanceLeft, float distanceRight, float distanceCenter)
{

    // Define maximum speed and safe distance thresholds
    const float maxSpeed = 100;        // Assuming 100 is full speed
    const float safeDistance = 80;     // Safe distance from obstacles in the same unit as sensor readings
    const float criticalDistance = 20; // Distance at which an immediate response is required

    // Initial assumption: no obstacles, go straight at full speed
    control.speed = maxSpeed;
    control.orientation = 0; // Straight

    // Check for obstacles
    if (distanceCenter < safeDistance || distanceLeft < safeDistance || distanceRight < safeDistance)
    {
        // There's an obstacle nearby, slow down
        float closestDistance = min(min(distanceLeft, distanceRight), distanceCenter);
        control.speed = map(closestDistance, criticalDistance, safeDistance, 0, maxSpeed);

        // Determine direction to turn based on which sensor detected the obstacle
        if (distanceLeft < distanceRight)
        {
            // Obstacle is closer to the left, turn right
            control.orientation = map(distanceLeft, criticalDistance, safeDistance, 0, 100);
        }
        else
        {
            // Obstacle is closer to the right, turn left
            control.orientation = map(distanceRight, criticalDistance, safeDistance, 0, -100);
        }
    }
    return control;
}