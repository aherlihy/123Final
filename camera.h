#ifndef CAMERA_H
#define CAMERA_H

#include "vector.h"

/**
    A basic camera
*/
struct MyCamera {
    Vector3 eye, center, up;
    float theta, phi;
    float fovy, near, far;


    void mouseMove(float deltaX, float deltaY);
    void mouseWheel(float delta);
    void lookAt(const Vector3 &point);
};

#endif // CAMERA_H
