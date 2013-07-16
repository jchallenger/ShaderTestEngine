/****************************************
Copyright HugePixels 2012

This File has been altered by Justin 
Challenger and Brandon Drenikow of 
HugePixels for use in Intro to Computer 
Graphics 2012.

This file is originally owned and created
by Justin Challenger.

2012
*****************************************/
#pragma once
#include "Quaternion.h"
#include "VectorOperations.h"
#include <GL/glew.h>

#ifndef M_PI
#define M_PI 3.141592f
#endif
#ifndef CAM_FPS
#define CAM_FPS 0
#endif
#ifndef CAM_OVR
#define CAM_OVR 1
#endif
#ifndef CAM_FREE
#define CAM_FREE 2
#endif
#ifndef CAM_FLW
#define CAM_FLW 3
#endif

class Camera
{
public:
    Camera(void);
    ~Camera(void);

    Vec3 position, lastPos, direction, pointOfFocus, up, right;

    void SetMode(int _mode);
    void SetFocus(Vec3 point);
    void SetPosition(Vec3 point);
    void MoveFocus(float speed, Vec3 &axis);
    void handleMouseMove(float mouseX, float mouseY, float screenWidth, float screenHeight);
    void draw();

    void rotateY(float amount);
    void rotateX(float amount);
    void rotateZ(float amount);

	void mForward(float delta);
	void mBackward(float delta);
	void mLeft(float delta);
	void mRight(float delta);

    //Quaternion Orientation
    int mode, speed;
    float xRot, lastRotX, lastRotY, yRot, zoom, sensitivity;

    bool wireFrame, LightsOn, TextureOn;
};