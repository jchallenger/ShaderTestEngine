#pragma once
#include <math.h>

class Vec3{
public:
    float x,y,z;

    Vec3(void);
    Vec3(float _x, float _y);
    Vec3(float _x, float _y, float _z);

    float length();
    float normalize();

    Vec3 set(float _x, float _y, float _z);
    Vec3 set(Vec3 &Vec);

    Vec3 operator=(const Vec3 &Vec);
    Vec3 operator+=(const Vec3 &Vec);
    Vec3 operator-=(const Vec3 &Vec);
    Vec3 operator*=(const float scalar);

    bool operator==(const Vec3 &Vec);
    bool operator!=(const Vec3 &Vec);

    Vec3 operator+(const Vec3 &Vec) const;
    Vec3 operator-(const Vec3 &Vec) const;
    Vec3 operator*(const Vec3 &Vec) const;
    Vec3 operator/(const Vec3 &Vec) const;

    Vec3 operator+(const float scalar) const;
    Vec3 operator-(const float scalar) const;
    Vec3 operator*(const float scalar) const;
    Vec3 operator/(const float scalar) const;

    friend Vec3 operator+(float f, Vec3 Vec){
        return Vec3(f+Vec.x,f+Vec.y,f+Vec.z);
    }
    friend Vec3 operator-(float f, Vec3 Vec){
        return Vec3(f-Vec.x,f-Vec.y,f-Vec.z);
    }
    friend Vec3 operator*(float f, Vec3 Vec){
        return Vec3(f*Vec.x,f*Vec.y,f*Vec.z);
    }

	float findAngleBetween(Vec3 &otherVector);

    Vec3 cross(const Vec3 &Vec) const;
    Vec3 lerp(float step, const Vec3 end) const;
    float dot(const Vec3 &Vec) const;

    bool isZero(){ return (x == 0 && y == 0 && z == 0); }
};

static float asm_sqrt (float value)
{
	_asm fld value
	_asm fsqrt
}

static float dot(const Vec3 &Vec,const Vec3 &Vec2){
    return (Vec.x * Vec2.x 
        + Vec.y * Vec2.y
        + Vec.z * Vec2.z);
}