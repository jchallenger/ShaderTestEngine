#include "Light.h"

short Light::numOfLights = 0;

Light::Light(void)
{
    backup = 0;

    Ambient[0] = 0.2f; Ambient[1] = 0.05f; Ambient[2] = 0.0f;    Ambient[3]  = 1.0f;
    Diffuse[0] = Diffuse[1] = Diffuse[2] = Diffuse[3] =	0.8f;
    Specular[0] = Specular[1] = Specular[2] = 0.7f; Specular[3] = 1.0f;

    setPos(Vec3(0,10,1));
    setDir(Vec3(0,-1,-1));

    //glEnable(GL_LIGHTING);
    //glLightModelfv(GL_LIGHT_MODEL_AMBIENT, Ambient);
    id = -1;
}


Light::~Light(void)
{
}
void Light::setShader(Shader * _shader){
    // Lights
    shader = _shader;
    /*char * attribute_name = "light_pos";
    shader->uni.light.position = glGetUniformLocation(shader->id(), attribute_name);
    if (shader->uni.light.position == -1) {
        printf("\nCould not bind uniform %s",attribute_name);
    }
    attribute_name = "light_dif";
    shader->uni.light.diffuse = glGetUniformLocation(shader->id(), attribute_name);
    if (shader->uni.light.diffuse == -1) {
        printf("\nCould not bind uniform %s",attribute_name);
    }
    attribute_name = "light_amb";
    shader->uni.light.ambience = glGetUniformLocation(shader->id(), attribute_name);
    if (shader->uni.light.ambience == -1) {
        printf("\nCould not bind uniform %s",attribute_name);
    }
    attribute_name = "light_spec";
    shader->uni.light.specular = glGetUniformLocation(shader->id(), attribute_name);
    if (shader->uni.light.specular == -1) {
        printf("\nCould not bind uniform %s",attribute_name);
    }*/
}
void Light::setBackup(){
    backup = new Light();
    backup->setAmb(Ambient);
    backup->setDif(Diffuse);
    backup->setSpec(Specular);
    backup->setPos(Position);
    backup->setDir(Direction);
}
void Light::off(){
    Ambient[0] = Ambient[1] = Ambient[2] = Ambient[3]  = 0.0f;
    Diffuse[0] = Diffuse[1] = Diffuse[2] = Diffuse[3] =	0.0f;
    Specular[0] = Specular[1] = Specular[2] = 0.0f; Specular[3] = 0.0f;
    //setPos(Vec3(0,0,0));
    if(id != -1)
        disable();
}
void Light::on(){
    setAmb( backup->getAmb());
    setDif( backup->getDif());
    setSpec(backup->getSpec());
    setPos( backup->getPos());
    setDir( backup->getDir());
    if(id == -1)
        enable();
}
void Light::setAmb(GLfloat * amb){
    Ambient[0] = amb[0];
    Ambient[1] = amb[1];
    Ambient[2] = amb[2];
    Ambient[3] = amb[3];
    if(backup != 0){
        backup->setAmb(amb);
    }
}
void Light::setPos(GLfloat * pos){
    Position[0] = pos[0];
    Position[1] = pos[1];
    Position[2] = pos[2];
    Position[3] = pos[3];
    if(backup != 0){
        backup->setPos(pos);
    }
}
void Light::setPos(const Vec3 &pos){
    Position[0] = pos.x;
    Position[1] = pos.y;
    Position[2] = pos.z;
    Position[3] = 1.0f;
    if(backup != 0){
        backup->setPos(pos);
    }
}
void Light::setDir(GLfloat * dir){
    Direction[0] = dir[0];
    Direction[1] = dir[1];
    Direction[2] = dir[2];
    Direction[3] = dir[3];
    if(backup != 0){
        backup->setDir(dir);
    }
}
void Light::setDir(const Vec3 &dir){
    Direction[0] = dir.x;
    Direction[1] = dir.y;
    Direction[2] = dir.z;
    Direction[3] = 1.0f;
    if(backup != 0){
        backup->setDir(dir);
    }
}
void Light::setSpec(GLfloat * spec){
    Specular[0] = spec[0];
    Specular[1] = spec[1];
    Specular[2] = spec[2];
    Specular[3] = spec[3];
    if(backup != 0){
        backup->setSpec(spec);
    }
}
void Light::setDif(GLfloat * dif){
    Diffuse[0] = dif[0];
    Diffuse[1] = dif[1];
    Diffuse[2] = dif[2];
    Diffuse[3] = dif[3];
    if(backup != 0){
        backup->setDif(dif);
    }
}
void Light::setDif(float r, float g, float b, float a){
    Diffuse[0] = r;
    Diffuse[1] = g;
    Diffuse[2] = b;
    Diffuse[3] = a;
    if(backup != 0){
        backup->setDif(Diffuse);
    }
}
void Light::enable(){
    id = numOfLights;
    numOfLights++;
}
void Light::disable(){
    numOfLights--;
    id = -1;
}

void Light::update(){
    //render();
}
void Light::render(){
    shader->use();
    
    if(id != -1 && shader->uni.numLightsActive != -1){
        glUniform1i(shader->uni.numLightsActive, numOfLights);
        glUniform4fv(shader->uni.lights[id].position, 1, Position);
        glUniform4fv(shader->uni.lights[id].diffuse,  1, Diffuse);
        glUniform4fv(shader->uni.lights[id].ambience, 1, Ambient);
        glUniform4fv(shader->uni.lights[id].specular, 1, Specular);
    }
    if(id == 0){
        if(shader->uni.light.position != -1){
            glUniform4fv(shader->uni.light.position,  1, Position);
        }
        if(shader->uni.light.diffuse != -1)
            glUniform4fv(shader->uni.light.diffuse,  1, Diffuse);
        if(shader->uni.light.ambience != -1)
            glUniform4fv(shader->uni.light.ambience, 1, Ambient);
        if(shader->uni.light.specular != -1)
            glUniform4fv(shader->uni.light.specular, 1, Specular);
    }
}