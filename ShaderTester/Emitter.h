#pragma once
#include "VectorOperations.h"
#include "TextureUtils.h"
#include <GL/glew.h>
#include <vector>
#include <ctime>
/*
 Justin Challenger
 Entity Emitter
 **********************************************************************
 Created:   November 28th 2012

 Description:
 This Emitter will emit Entities based on time, and behaviour.
 I will have a set Type, ad perhaps read in from a file the individual
 properties for each particle.

 The entities will then be sorted in a LinkedList or tree of somesort
 and updated accordingly.

 **********************************************************************
 Updated:   November 28th 2012
*/
class Emitter;
class Particle
{
public:
    Particle();
    ~Particle();

    void update(float delta);
    void draw();

    bool dead;
    float speed, lifetimeSecs;
    Vec3 position, velo;
    float accel;

    GLuint texture;
    float R,G,B,A;

    Emitter * emitter;
};
class Emitter
{
public:
    Emitter(void);
    ~Emitter(void);

    void createAllParticles();
    void update(float delta);
    void draw();

    int type;
    Vec3 pos;

    std::vector<Particle*> particles;
};

