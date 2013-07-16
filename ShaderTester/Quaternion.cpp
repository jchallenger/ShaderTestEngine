#include "Quaternion.h"

Quaternion::Quaternion(void){
    w = 0;
    set(0,0,0);
    magnitude = 0;
}
Quaternion::Quaternion(Vec3 vec){
    w = 0;
    set(vec);
    magnitude = this->length();
}
Quaternion::Quaternion(float x, float y, float z){
    w = 0;
    set(x,y,z);
    magnitude = this->length();
}
Quaternion::Quaternion(float _w, Vec3 vec){
    w = _w;
    set(vec);
    magnitude = this->length();
}
Quaternion::Quaternion(float _w, float x, float y, float z){
    w = _w;
    set(x,y,z);
    magnitude = this->length();
}

Quaternion::~Quaternion(void)
{
}

Quaternion Quaternion::rotateAround(Vec3 axis, float radians){
    Quaternion rotated = *this;
    Quaternion rotation(cos(radians/2),(axis*sin(radians/2)));
    Quaternion rotationConj(rotation);
    rotationConj.inverse();

    *this = rotation*rotated*rotationConj;

    this->normalize();

    return *this;
}
Quaternion slerp(Quaternion const &q1, Quaternion const &q2, float t){
    Quaternion result;
    float d = dot(q1,q2);
	float angle = acos(t);

	if (fabs(angle) >= 1e-06f)
	{
		float sn = sin(angle);
		float invSn = ((float)1)/sn;
		float tAngle = t*angle;
		float coeff0 = sin(angle - tAngle)*invSn;
		float coeff1 = sin(tAngle)*invSn;

		result.x = coeff0*q1.x + coeff1*q2.x;
		result.y = coeff0*q1.y + coeff1*q2.y;
		result.z = coeff0*q1.z + coeff1*q2.z;
		result.w = coeff0*q1.w + coeff1*q2.w;
	}
	else
	{
		result = q1;
	}

    return result;
}
float * Quaternion::toMat4() const{
    float result[] = {
        1-(2*y*y)-(2*z*z),  2*x*y-2*w*z,    2*x*z+2*w*y,    0,
        2*x*y+2*w*z,        1-2*x*x-2*z*z,  2*y*z+2*w*x,    0,
        2*x*z-2*w*y,        2*y*z-2*w*x,    1-2*x*x-2*y*y,  0,
        0,                  0,              0,              1
    };
    return result;
}
Vec3 Quaternion::toVector3f() const{
    return Vec3(x,y,z);
}

float Quaternion::normalize(){
    this->length();

    if(magnitude != 0.0f){
        w /= magnitude;
        x /= magnitude;
        y /= magnitude;
        z /= magnitude;
    }
    return magnitude;
}
float Quaternion::length(){
    magnitude = sqrt((w*w)+(x*x)+(y*y)+(z*z));
    return magnitude;
}
void Quaternion::inverse(){
    x *= -1;
    y *= -1;
    z *= -1;
}

Quaternion Quaternion::operator*(const Quaternion &quat2) const{
    Quaternion quat1 = *this, newQ;

    newQ.w = quat1.w * quat2.w - quat1.x * quat2.x - quat1.y * quat2.y - quat1.z * quat2.z;
    newQ.x = quat1.w * quat2.x + quat1.x * quat2.w + quat1.y * quat2.z - quat1.z * quat2.y;
    newQ.y = quat1.w * quat2.y - quat1.x * quat2.z + quat1.y * quat2.w + quat1.z * quat2.x;
    newQ.z = quat1.w * quat2.z + quat1.x * quat2.y - quat1.y * quat2.x + quat1.z * quat2.w;
    newQ.length();


    return newQ;
}
Quaternion Quaternion::operator=(const Quaternion &quat){
    w = quat.w;
    x = quat.x;
    y = quat.y;
    z = quat.z;
    magnitude = quat.magnitude;
    return *this;
}