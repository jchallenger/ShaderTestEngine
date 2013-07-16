#pragma once
#include "VectorOperations.h"
#include "shader_Utils.h"
#include <GL/glew.h>


class Light
{
public:
    Light(void);
    ~Light(void);

    void setAmb(GLfloat *);
    void setDif(GLfloat *);
    void setDif(float r, float g, float b, float a=1);

    void setPos(GLfloat *);
    void setPos(const Vec3 &pos);
    void setSpec(GLfloat *);
    void setDir(GLfloat *);
    void setDir(const Vec3 &dir);
    void setShader(Shader * _shader);
    void setBackup();
    void off(); void on();

    GLfloat * getAmb() { return Ambient; }
    GLfloat * getDif() { return Diffuse; }
    GLfloat * getPos() { return Position; }
    GLfloat * getSpec(){ return Specular; }
    GLfloat * getDir() { return Direction; }

    void enable();
    void disable();
    void update();
    void render();

    int id;
    static short numOfLights;

private:
    Light * backup;     // hold original values
    GLfloat Ambient[4];
    GLfloat Diffuse[4];
    GLfloat Position[4];
    GLfloat Specular[4];
    GLfloat Direction[4];
    Shader * shader;
};

