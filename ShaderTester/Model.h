#pragma once
/*
 Justin Challenger
 Basic Model Class (OBJ)
 **********************************************************************
 Created:   November 17th 2012

 Description:
 A basic OBJ loader/drawing class. 

 TODO:
 Textures

 **********************************************************************
 Updated:   November 28th 2012
*/

#include <string>   // To open names
#include <fstream>  // To load
#include <iostream>
#include <vector>
#include "TextureUtils.h"
#include "shader_Utils.h"
#include <GL/glew.h>

#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

class Entity;
typedef struct Material{
    float dif[3], amb[3], spec[3];
    float alpha; // 1 = opaque
    float ns; // Specular brightness

    Material();
    Material(const char* _name, float _al, float _ns, float * _di, float * _am, float *_sp, int ill);
    int illum;
    std::string name;
}_Material;
typedef struct Face{
    int v_ind[3], t_ind[3], n_ind[3];
    int mat;
}_Face;
typedef struct xyz{
    float x, y, z;
    xyz(float _x, float _y, float _z){
        x = _x; y = _y; z = _z;
    }
}_xyz;
typedef struct UVmap{
    float u,v;
    UVmap(float _u, float _v){
        u = _u; v = _v;
    }
}_UVmap;
class Model
{
public:
    Model(void);
    Model(std::string file);
    ~Model(void);
    bool load(std::string filename);
    bool loadMats(std::string filename);
    void draw();
    void drawIM();
    void bind(Shader * shader);
    void drawWire();
    Material getMaterial0();

    GLuint vao, buffers[4];                     // VAO and VAO Buffer
    GLuint numOfVerts, numOfNorms, numOfUVs;    // IBO base pointers
    GLuint drawArraySize;
    
    bool morphing;
    Entity * parentEntity;

    std::vector<GLfloat> vBuff, tBuff, nBuff;
    std::vector<GLushort> fBuff;

private:
    bool hasMaterial;
    GLenum errC;
    std::string name;
    std::vector<UVmap> UVs;
    std::vector<xyz> verts, norms;
    std::vector<Material> materials;
    std::vector<Face> faces;
};

