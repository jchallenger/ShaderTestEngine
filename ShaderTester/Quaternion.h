#pragma once
#include "vectoroperations.h"

class Quaternion :
    public Vec3
{
public:
    Quaternion(void);
    Quaternion(Vec3 vec);
    Quaternion(float x, float y, float z);
    Quaternion(float _w, Vec3 vec);
    Quaternion(float _w, float x, float y, float z);
    ~Quaternion(void);

    Quaternion rotateAround(Vec3 axis, float radians);
    Quaternion slerp(Quaternion const &q1, Quaternion const &q2, float t);

    Vec3 toVector3f() const;

    float normalize();
    float length();
    void inverse();
    float dot(const Quaternion &q2) const;

    //Matrix3 toMat3() const;
    float * toMat4() const;

    Quaternion operator*(const Quaternion &quat2) const;
    Quaternion operator=(const Quaternion &quat);

    // Members
    float w, magnitude;
};

static float dot(const Quaternion &q1,const Quaternion &q2){
    return(dot(q1.toVector3f(),q2.toVector3f())+q1.w*q2.w);
}