/*!
   @file   CS123Matrix.cpp
   @author Travis Fischer (fisch0920@gmail.com)
   @date   Fall 2008
   
   @brief
      Provides basic functionality for a templated, arbitrarily-sized matrix.
      You will need to fill this file in for the Camtrans assignment.

**/

#include "CS123Algebra.h"
#include <iostream>

//@name Routines which generate specific-purpose transformation matrices
//@{---------------------------------------------------------------------

// @returns the scale matrix described by the vector
Matrix4x4 getScaleMat(const Vector4 &v) {

    // @TODO: [CAMTRANS] Fill this in...
    return mat4<REAL>(v.x, 0, 0, 0,
                      0, v.y, 0, 0,
                      0, 0, v.z, 0,
                      0, 0, 0,  1);
}

// @returns the translation matrix described by the vector
Matrix4x4 getTransMat(const Vector4 &v) {

    // @TODO: [CAMTRANS] Fill this in...
    return mat4<REAL>(1, 0, 0, v.x,
                      0, 1, 0, v.y,
                      0, 0, 1, v.z,
                      0, 0, 0,  1);

}

// @returns the rotation matrix about the x axis by the specified angle
Matrix4x4 getRotXMat (const REAL radians) {

    // @TODO: [CAMTRANS] Fill this in...
    REAL x = cos(radians);
    REAL y = sin(radians);
    return mat4<REAL>(1, 0, 0, 0,
                      0, x,-y, 0,
                      0, y, x, 0,
                      0, 0, 0, 1);

}

// @returns the rotation matrix about the y axis by the specified angle
Matrix4x4 getRotYMat (const REAL radians) {

    // @TODO: [CAMTRANS] Fill this in...
    REAL x = cos(radians);
    REAL y = sin(radians);
    return mat4<REAL>(x, 0, y, 0,
                      0, 1, 0, 0,
                     -y, 0, x, 0,
                      0, 0, 0, 1);
}

// @returns the rotation matrix about the z axis by the specified angle
Matrix4x4 getRotZMat (const REAL radians) {

    // @TODO: [CAMTRANS] Fill this in...
    REAL x = cos(radians);
    REAL y = sin(radians);
    return mat4<REAL>(x,-y, 0, 0,
                      y, x, 0, 0,
                      0, 0, 1, 0,
                      0, 0, 0, 1);
}

// @returns the rotation matrix around the vector and point by the specified angle
Matrix4x4 getRotMat  (const Vector4 &point, const Vector4 &vector, const REAL angle) {

    // @TODO: [CAMTRANS] Fill this in...
    REAL theta = atan2(vector.z, vector.x);
    REAL phi = -atan2(vector.y, sqrt((vector.x*vector.x + vector.z*vector.z)));
    Matrix4x4 p0 = getTransMat(point);
    Matrix4x4 p1 = getInvTransMat(point);
    Matrix4x4 M1 = getRotYMat(theta);
    Matrix4x4 M2 = getRotZMat(phi);
    Matrix4x4 M3 = getRotXMat(angle);
    Matrix4x4 M1_i = getInvRotYMat(theta);
    Matrix4x4 M2_i = getInvRotZMat(phi);
    return p0*M1_i*M2_i*M3*M2*M1*p1;

}


// @returns the inverse scale matrix described by the vector
Matrix4x4 getInvScaleMat(const Vector4 &v) {

    // @TODO: [CAMTRANS] Fill this in...
    return getScaleMat(v).invert();

}

// @returns the inverse translation matrix described by the vector
Matrix4x4 getInvTransMat(const Vector4 &v) {

    // @TODO: [CAMTRANS] Fill this in...
    return getTransMat(v).invert();

}

// @returns the inverse rotation matrix about the x axis by the specified angle
Matrix4x4 getInvRotXMat (const REAL radians) {

    // @TODO: [CAMTRANS] Fill this in...
    return getRotXMat(radians).invert();


}

// @returns the inverse rotation matrix about the y axis by the specified angle
Matrix4x4 getInvRotYMat (const REAL radians) {

    // @TODO: [CAMTRANS] Fill this in...
    return getRotYMat(radians).invert();

}

// @returns the inverse rotation matrix about the z axis by the specified angle
Matrix4x4 getInvRotZMat (const REAL radians) {

    // @TODO: [CAMTRANS] Fill this in...
    return getRotZMat(radians).invert();

}

// @returns the inverse rotation matrix around the vector and point by the specified angle
Matrix4x4 getInvRotMat  (const Vector4 &p, const Vector4 &v, const REAL a) {

    // @TODO: [CAMTRANS] Fill this in...
    return getRotMat(p, v, a).invert();

}


//@}---------------------------------------------------------------------

