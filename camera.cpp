
#include "camera.h"
#include <qgl.h>
#include <iostream>

using namespace std;

void MyCamera::mouseMove(float deltaX, float deltaY)
{
//    // Rotate the eye vector around the origin
//    theta += deltaX * 0.01f;
//    phi += deltaY * 0.01f;

//    // Keep theta in [0, 2pi] and phi in [-pi/2, pi/2]
//    theta -= floorf(theta / (2.0*M_PI)) * (2.0*M_PI);
//    phi = max(0.01f - M_PI / 2, min(M_PI / 2 - 0.01f, phi));
}

void MyCamera::mouseWheel(float delta)
{
//    center += -Vector3::fromAngles(theta, phi)  * delta;
}

void MyCamera::lookAt(const Vector3 &point)
{
//    /**
//     * Spherical coordinate! http://en.wikipedia.org/wiki/Spherical_coordinate_system#Cartesian_coordinates
//     * note, we are using y as the up vector as opposed to z which changes the equation slightly
//     */
//    Vector3 dir = point.unit();
//    phi = asinf(dir.y);
//    theta = atanf(dir.z/dir.x);
}

