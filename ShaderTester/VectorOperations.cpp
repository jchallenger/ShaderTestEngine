#include "VectorOperations.h"

Vec3::Vec3(){
    x = 0;
    y = 0;
    z = 0;
}
Vec3::Vec3(float _x, float _y){
    x = _x;
    y = _y;
    z = 0;
}
Vec3::Vec3(float _x, float _y, float _z)
{
    x = _x;
    y = _y;
    z = _z;
}

float Vec3::length()
{
    return sqrt((x*x)+(y*y)+(z*z));
}
float Vec3::normalize()
{
    float length = this->length();
    x = x/length;
    y = y/length;
    z = z/length;
    return length;
}

Vec3 Vec3::set(float _x, float _y, float _z){
    x = _x;
    y = _y;
    z = _z;
    return *this;
}
Vec3 Vec3::set(Vec3 &Vec){
    x = Vec.x;
    y = Vec.y;
    z = Vec.z;
    return *this;
}

Vec3 Vec3::cross(const Vec3 &Vec) const{
    Vec3 crossVec;
    crossVec.x = y*Vec.z - z*Vec.y; 
    crossVec.y = z*Vec.x - x*Vec.z;
    crossVec.z = x*Vec.y - y*Vec.x;
    return crossVec;
}
Vec3 Vec3::lerp(const float step, const Vec3 end) const{
    return *this*(1-step)+end*2*step;
}
float Vec3::dot(const Vec3 &Vec) const{
    return (x*Vec.x+y*Vec.y+z*Vec.z);
}


float Vec3::findAngleBetween(Vec3 &otherVec)
{
	return acos(this->dot(otherVec) / this->length() / otherVec.length());
}

// OPERATOR OVERLOAD

Vec3 Vec3::operator=(const Vec3 &Vec){
    x = Vec.x;
    y = Vec.y;
    z = Vec.z;
    return *this;
}
Vec3 Vec3::operator+=(const Vec3 &Vec){
    x += Vec.x;
    y += Vec.y;
    z += Vec.z;
    return *this;
}
Vec3 Vec3::operator-=(const Vec3 &Vec){
    x -= Vec.x;
    y -= Vec.y;
    z -= Vec.z;
    return *this;
}
Vec3 Vec3::operator*=(const float scalar){
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

bool Vec3::operator==(const Vec3 &Vec){
    if( x == Vec.x &&
        y == Vec.y &&
        z == Vec.z)
        return true;
    else
        return false;
}
bool Vec3::operator!=(const Vec3 &Vec){
    if( x == Vec.x &&
        y == Vec.y &&
        z == Vec.z)
        return false;
    else
        return true;
}

Vec3 Vec3::operator+(const Vec3 &Vec)const{
        return Vec3(this->x + Vec.x, this->y + Vec.y, this->z + Vec.z);
}
Vec3 Vec3::operator-(const Vec3 &Vec)const{
    return Vec3(this->x - Vec.x, this->y - Vec.y, this->z - Vec.z);
}
Vec3 Vec3::operator*(const Vec3 &Vec)const{
        return Vec3(this->x * Vec.x, this->y * Vec.y, this->z * Vec.z);
}
Vec3 Vec3::operator/(const Vec3 &Vec)const{
        return Vec3(this->x / Vec.x, this->y / Vec.y, this->z / Vec.z);
}

Vec3 Vec3::operator+(const float scalar )const{
    return Vec3(this->x + scalar, this->y + scalar, this->z + scalar);
}
Vec3 Vec3::operator-(const float scalar )const{
    return Vec3(this->x - scalar, this->y - scalar, this->z - scalar);
}
Vec3 Vec3::operator*(const float scalar )const{
    return Vec3(this->x * scalar, this->y * scalar, this->z * scalar);
}
Vec3 Vec3::operator/(const float scalar )const{
    return Vec3(this->x / scalar, this->y / scalar, this->z / scalar);
}