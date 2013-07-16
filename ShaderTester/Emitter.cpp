#include "Emitter.h"


Emitter::Emitter(void)
{
}
Particle::Particle(){
    position.set(0,0,0);
    accel = 0;
}

Emitter::~Emitter(void)
{
}
void Emitter::createAllParticles(){
    int i;
    int ParticleCount = 500;
	for (i = 1; i < ParticleCount; i++)
	{
        srand((unsigned int)time(0));

        Particle *temp = new Particle();
        temp->emitter = this;
		temp->position = pos;
        temp->velo = Vec3(rand()%100 -50.0f, rand()%100 -50.0f,rand()%100 -50.0f);

        temp->R = 1.f;
		temp->G = 1.f;
		temp->B = 1.f;
        temp->A = 1.f;

		temp->accel = ((((((8 - 5 + 2) * rand()%11) + 5) - 1 + 1) * rand()%11) + 1) * 0.02f;
        particles.push_back(temp);
	}
}
void Particle::draw(){
    //glPushMatrix();

	glTranslatef (position.x, position.y, position.z);

	glDisable (GL_DEPTH_TEST);
	glEnable (GL_BLEND);

	glBlendFunc (GL_DST_COLOR, GL_ZERO);
	glBindTexture (GL_TEXTURE_2D, texture);
    glColor4f (R,G,B,0);
	glBegin (GL_QUADS);
	glTexCoord2d(0.0,0.0); 
	glVertex2d(-1.0,-1.0);
	glTexCoord2d(1.0,0.0); 
	glVertex2d(1.0,-1.0);
	glTexCoord2d(1.0,1.0); 
	glVertex2d(1.0,1.0);
	glTexCoord2d(0.0,1.0); 
	glVertex2d(-1.0,1.0);
	glEnd();

    //glPopMatrix();
}
void Particle::update(float delta){
	position = Vec3(position.x + velo.x, position.y + accel, position.z + velo.z );
    accel = accel * 0.8f;

	//Particle[i].Direction = Particle[i].Direction + ((((((int)(0.5 - 0.1 + 0.1) * rand()%11) + 1) - 1 + 1) * rand()%11) + 1);

	if (position.y < -50)
	{
        position = emitter->pos;
        accel = ((((((8 - 5 + 2) * rand()%11) + 5) - 1 + 1) * rand()%11) + 1) * 0.02;
	}
}
void Emitter::update(float delta){
    for(int i = 0 ; i < particles.size(); i++){
        particles[i]->update(delta);
    }
}
void Emitter::draw(){
    for(int i = 0 ; i < particles.size(); i++){
        particles[i]->draw();
    }
}