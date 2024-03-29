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
    return Matrix4x4(v.x, 0,   0,   0,
                     0,   v.y, 0,   0,
                     0,   0,   v.z, 0,
                     0,   0,   0,   1);
}

// @returns the translation matrix described by the vector
Matrix4x4 getTransMat(const Vector4 &v) {
    return Matrix4x4(1, 0, 0, v.x,
                     0, 1, 0, v.y,
                     0, 0, 1, v.z,
                     0, 0, 0, 1);
}

// @returns the rotation matrix about the x axis by the specified angle
Matrix4x4 getRotXMat (const REAL radians) {
    return Matrix4x4(1, 0,            0,             0, 
                     0, cos(radians), -sin(radians), 0,
                     0, sin(radians), cos(radians),  0,
                     0, 0,            0,             1);
}

// @returns the rotation matrix about the y axis by the specified angle
Matrix4x4 getRotZMat (const REAL radians) {
    return Matrix4x4(cos(radians), -sin(radians), 0, 0,
                     sin(radians), cos(radians),  0, 0,
                     0,            0,             1, 0,
                     0,            0,             0, 1);
}
// @returns the rotation matrix about the z axis by the specified angle
Matrix4x4 getRotYMat (const REAL radians) {
    return Matrix4x4(cos(radians),  0, sin(radians), 0,
                     0,             1, 0,            0,
                     -sin(radians), 0, cos(radians), 0, 
                     0,             0, 0,            1);
}

// @returns the rotation matrix around the vector and point by the specified angle
Matrix4x4 getRotMat  (const Vector4 &p, const Vector4 &v, const REAL a) {
    Matrix4x4 rotmat = Matrix4x4::identity();

    double theta = atan2(v.z, v.x);
    double phi = -atan2(v.y, sqrt(v.x*v.x + v.z*v.z));

    // this is basically the same as one of the algo problems
    return getTransMat(p) *       //  tinv
           getInvRotYMat(theta) * //  m1inv
           getInvRotZMat(phi) *   //  m2inv
           getRotXMat(a) *        //  m3
           getRotZMat(phi) *      //  m2
           getRotYMat(theta) *    //  m1
           getInvTransMat(p);     //  t 
}

// I used the conceptual inverses of the inputs of the original transformation matrices
// to compute the inverses without using linear algebra

// @returns the inverse scale matrix described by the vector
Matrix4x4 getInvScaleMat(const Vector4 &v) {
    return getScaleMat(Vector4(1,1,1,1) / v);
}

// @returns the inverse translation matrix described by the vector
Matrix4x4 getInvTransMat(const Vector4 &v) {
    return getTransMat(-v);
}

// @returns the inverse rotation matrix about the x axis by the specified angle
Matrix4x4 getInvRotXMat (const REAL radians) {
    return getRotXMat(-radians);
}

// @returns the inverse rotation matrix about the y axis by the specified angle
Matrix4x4 getInvRotZMat (const REAL radians) {
    return getRotZMat(-radians);
}

// @returns the inverse rotation matrix about the z axis by the specified angle
Matrix4x4 getInvRotYMat (const REAL radians) {
    return getRotYMat(-radians);
}

// @returns the inverse rotation matrix around the vector and point by the specified angle
Matrix4x4 getInvRotMat  (const Vector4 &p, const Vector4 &v, const REAL a) {
    return getRotMat(p, v, -a);
}


//@}---------------------------------------------------------------------

