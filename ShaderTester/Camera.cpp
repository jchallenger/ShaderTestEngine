#include "Camera.h"

Camera::Camera(void)
{
    SetPosition(Vec3(0,0,0));
    up.set(0,1,0);
    right.set(-1,0,0);
    direction.set(0,0,1);

    this->SetFocus(Vec3(0,0,-4));

    mode = CAM_FREE;
    xRot = yRot = 0;
    sensitivity = zoom = 1;
    speed = 100;
}
Camera::~Camera(void)
{
}

void Camera::SetMode(int _mode){
    mode = _mode;
    switch(mode){
    case CAM_OVR:
    case CAM_FREE:
    case CAM_FPS:
        break;
    case CAM_FLW:/*
        SetFocus(Vec3(position.x,position.y,position.z+1));*/
        break;
    }
}
void Camera::SetFocus(Vec3 point){
    pointOfFocus = point;
    direction = pointOfFocus - position;
    direction.normalize();
    right = direction.cross(up);
    right.normalize();
    /*if(mode == CAM_FLW)
    {
        right += direction;
    }*/
}
void Camera::SetPosition(Vec3 point){
    position = point;
    if(mode == CAM_OVR){
        direction.set(0,-1,-0.5);
        up.set(0,0,-1);
    }
    SetFocus(position + direction);
}
void Camera::MoveFocus(float speed, Vec3 &axis){
    Vec3 temp;
    temp = axis;
    temp.normalize();
    temp *= speed;
    pointOfFocus += temp;
}

void Camera::handleMouseMove(float mouseX, float mouseY, float screenWidth, float screenHeight)
{
    float mX=mouseX, mY=mouseY;
    // Convert mouse movements to radians
    mX -=  screenWidth/2;
    mY -=  screenHeight/2;

    mX /= 180.f/M_PI;
    mY /= 180.f/M_PI;

    xRot -= mX;
    yRot -= mY;

    // Keeps rotation within reasonable degrees
    if(xRot < -2*M_PI)
        xRot += 2*M_PI;
    if(xRot > 2*M_PI)
        xRot -= 2*M_PI;

    // Limits rotation on Y to 90 and -90 degrees
    if(yRot < -M_PI/2)
        yRot = -M_PI/2;
    if(yRot > M_PI/2)
        yRot = M_PI/2;
    
    // X, Y, and Z vectors for rotating around
    Quaternion PoF(0,0,1);
    Quaternion RiV(-1,0,0);
    
    PoF.rotateAround(up, xRot);
    RiV.rotateAround(up, xRot);
    PoF.rotateAround(right, yRot);

    // Normalize
    RiV.normalize();
    PoF.normalize();

    // Sets Camera
    SetFocus(position + PoF.toVector3f());
    right = RiV.toVector3f();    
}
void Camera::draw(){
    if(this->LightsOn){
        glEnable(GL_LIGHTING);
    }
    else{
        glDisable(GL_LIGHTING);
    }
    if(this->wireFrame){
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else{
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    if(this->TextureOn){
        glEnable(GL_TEXTURE_2D);
    }
    else{
        glDisable(GL_TEXTURE_2D);
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90, 1, 0.1, 1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        position.x, position.y, position.z,
        pointOfFocus.x, pointOfFocus.y, pointOfFocus.z,
        up.x,up.y,up.z);

}

void Camera::mForward(float delta){
	position        += direction * speed * delta;
	pointOfFocus    += direction * speed * delta;
}
void Camera::mBackward(float delta){
	position        -= direction * speed * delta;
	pointOfFocus    -= direction * speed * delta;
}
void Camera::mRight(float delta){
	position        += right * speed * delta;
	pointOfFocus    += right * speed * delta;
}
void Camera::mLeft(float delta){
	position        -= right * speed * delta;
	pointOfFocus    -= right * speed * delta;
}
/*void Camera::handleMouseMove(int mouseX, int mouseY, struct _GameStateInfo &info, Objects *player)
{
    float mX=(float)mouseX, mY=(float)mouseY;
    // Convert mouse movements to radians
    mX -= info.screenWidth/2;
    mY -= info.screenHeight/2;

    mX /= 180/M_PI;
    mY /= 180/M_PI;

    xRot -= mX;
    yRot += mY;

    // Keeps rotation within reasonable degrees
    if(xRot < -2*M_PI)
        xRot += 2*M_PI;
    if(xRot > 2*M_PI)
        xRot -= 2*M_PI;

    // Limits rotation on Y to 90 and -90 degrees
    if(yRot < -M_PI/2)
        yRot = -M_PI/2;
    if(yRot > M_PI/2)
        yRot = M_PI/2;
    
    // Only preforms operations if we NEED to
    if(lastRotX != xRot || lastRotY != yRot || player->lastPos != player->position) // Replace with player velocity?
    {
        // X, Y, and Z vectors for rotating around
        Quaternion PoF(0,0,1);
        Quaternion RiV(1,0,0);
    
        PoF.rotateAround(up, xRot);
        RiV.rotateAround(up, xRot);
        PoF.rotateAround(right, yRot);

        // Normalize
        RiV.normalize();
        PoF.normalize();

        // Change Position...
        if(this->mode == CAM_FLW){
            SetPosition(player->position - (PoF.toVector3f()*zoom));
            SetFocus(player->position);
            right = RiV.toVector3f();
        }
        else
        {
            // Rotates Player at the same rate as the Camera
            player->xRot = xRot;

            SetFocus(position + PoF.toVector3f());
            right = RiV.toVector3f();
        }

        lastRotX = xRot;
        lastRotY = yRot;
        player->lastPos=player->position;
    }
    // Reset the mouse position to the centre of the window each frame
    
}

void Camera::MoveCamera(float speed, struct _Movement &m, Objects *player){
    //Moves camera according to the camera's direction, right, and up Vector3fs
    switch(mode)
    {
    case CAM_FLW:
    case CAM_FPS:
        // Should limit movements to along X and Z axis, exclude Y
        if(m.MOVE_FORWARD)
        {
            player->position.x += direction.x * speed;
            player->position.z += direction.z * speed;
        }
        if(m.MOVE_LEFT)
        {
            player->position.x += right.x * speed;
            player->position.z += right.z * speed;
        }
        if(m.MOVE_BACKWARD)
        {
            player->position.x -= direction.x * speed;
            player->position.z -= direction.z * speed;
        }
        if(m.MOVE_RIGHT)
        {
            player->position.x -= right.x * speed;
            player->position.z -= right.z * speed;
        }
        if(mode == CAM_FPS)
        {
            position = (player->position + player->center) + (player->direction*player->scale);
            SetFocus(position + direction);
        }
        else
        {
            // Rotates Player at the same rate as the Camera
            player->xRot = xRot;
        }
        break;

    case CAM_OVR:
        if(m.MOVE_FORWARD)
        {
            position.x += up.x * speed;
            position.z += up.z * speed;
        }
        if(m.MOVE_LEFT)
        {
            position.x -= right.x * speed;
            position.z -= right.z * speed;
        }
        if(m.MOVE_BACKWARD)
        {
            position.x -= up.x * speed;
            position.z -= up.z * speed;
        }
        if(m.MOVE_RIGHT)
        {
            position.x += right.x * speed;
            position.z += right.z * speed;
        }
        SetFocus(position + direction);
        break;
    case CAM_FREE:
        // Camera movements should be free, with no restraints
        if(m.MOVE_FORWARD)
            position += direction * speed;
        if(m.MOVE_LEFT)
            position += right * speed;
        if(m.MOVE_BACKWARD)
            position -= direction * speed;
        if(m.MOVE_RIGHT)
            position -= right * speed;
        SetFocus(position + direction);
        break;
    }
    
    
}
*/

// None of these are actually used right now...
void Camera::rotateY(float amount){

    Vec3 target(direction);
    Vec3 newRight(right);

    SetFocus(Vec3(
        (cos((M_PI/2)+amount)*target.x)+(cos(amount)*newRight.x),
        (cos((M_PI/2)+amount)*target.y)+(cos(amount)*newRight.y),
        (cos((M_PI/2)+amount)*target.z)+(cos(amount)*newRight.z)));

    
    right.set(
        (cos(amount)*target.x)+(cos((M_PI/2)-amount)*newRight.x),
        (cos(amount)*target.y)+(cos((M_PI/2)-amount)*newRight.y),
        (cos(amount)*target.z)+(cos((M_PI/2)-amount)*newRight.z));

    //pointOfFocus.normalize();
    right.normalize();

}
void Camera::rotateX(float amount){

    Vec3 target(direction);
    Vec3 newRight(up);

    SetFocus(Vec3(
        (cos((M_PI/2)+amount)*target.x)+(cos((float)amount)*newRight.x),
        (cos((M_PI/2)+amount)*target.y)+(cos((float)amount)*newRight.y),
        (cos((M_PI/2)+amount)*target.z)+(cos((float)amount)*newRight.z)));

    up.set(
        (cos((float)amount)*target.x)+(cos((M_PI/2)-amount)*newRight.x),
        (cos((float)amount)*target.y)+(cos((M_PI/2)-amount)*newRight.y),
        (cos((float)amount)*target.z)+(cos((M_PI/2)-amount)*newRight.z));

    //pointOfFocus.normalize();
    up.normalize();
}
void Camera::rotateZ(float amount){

    Vec3 target(up);
    Vec3 newRight(right);

    up.set(
        (cos((M_PI/2)+amount)*target.x)+(cos((float)amount)*newRight.x),
        (cos((M_PI/2)+amount)*target.y)+(cos((float)amount)*newRight.y),
        (cos((M_PI/2)+amount)*target.z)+(cos((float)amount)*newRight.z));

    right.set(
        (cos((float)amount)*target.x)+(cos((M_PI/2)-amount)*newRight.x),
        (cos((float)amount)*target.y)+(cos((M_PI/2)-amount)*newRight.y),
        (cos((float)amount)*target.z)+(cos((M_PI/2)-amount)*newRight.z));

    direction.normalize();
    right.normalize();
}