#include "Entity.h"
#include "Scene.h"

Entity::Entity(void)
{
    currentModel = NULL;
    targetMorph  = NULL;
    init();
}
Entity::Entity(std::string filename, Shader * sh)
{
    std::cout <<" \n============================== ";
    std::cout << "\nLoading " << filename;
    setShader(sh);
    addModel(filename);
    currentModel = animList[0];
    targetMorph  = animList[0];
    init();
    
}
void Entity::init(){
    animTimer.currAnimTime = 0;
    animTimer.totalTime = 0;
    animTimer.cycleDone = false;
    animTimer.cycleRepeat = true;
    animTimer.cycleStart = true;
    animTimer.nextAnim = 0;
    animTimer.duration = 1.0f;

    position.set(0,0,0);
    scale.set(1,1,1);
    xRot = yRot = zRot = 0;
    textureType = DIF_MAP;
}
void Entity::addModel(std::string filename){
    Model * temp = new Model();
    temp->parentEntity = this;
    temp->load(filename);
    if(shader->id() != NULL){
        temp->bind(shader);
    }
    animList.push_back(temp);
}

Entity::~Entity(void)
{
}
void Entity::setShader(Shader * newShader){
    shader = newShader;

    std::vector<Model *>::iterator it;
    for(it = animList.begin(); it != animList.end(); it++){
        Model * sh = (*it);
        sh->bind(shader);
    }
}
void Entity::setScene(Scene * _scene){
    scene = _scene;
    drawMode = scene->drawMode;
}
_AnimTime * Entity::getAnimTimer(){
    return &animTimer;
}
void Entity::update(float delta){
    animTimer.totalTime += delta;

    if(animTimer.cycleStart){
        animTimer.currAnimTime += delta;
        if(animTimer.currAnimTime >= animTimer.duration){
            animTimer.currAnimTime -= animTimer.duration;

            if(animList.size() > animTimer.nextAnim + 1){
                animTimer.nextAnim++;
                if( animTimer.cycleDone && !animTimer.cycleRepeat){
                    animTimer.cycleStart = false;
                }
            }
            else{
                animTimer.nextAnim = 0;
                animTimer.cycleDone = true;
            }

            //glBindBuffer(GL_ARRAY_BUFFER, currentModel->buffers[0]);
            //glBufferSubData(GL_ARRAY_BUFFER, 1, sizeof(GLfloat)*(currentModel->vBuff.size()), &targetMorph->vBuff[3]);
            currentModel = targetMorph;
            targetMorph = animList[animTimer.nextAnim];

            if(shader->attr.coord3d2 != -1){
                glBindBuffer(GL_ARRAY_BUFFER, currentModel->buffers[MORPH_VERTICES]);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)*currentModel->vBuff.size(), &targetMorph->vBuff[0]);
            }
            if(shader->attr.normals2 != -1){
                glBindBuffer(GL_ARRAY_BUFFER, currentModel->buffers[MORPH_NORMALS]);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)*currentModel->nBuff.size(), &targetMorph->nBuff[0]);
            }
            if(shader->attr.texcoord2 != -1){
                glBindBuffer(GL_ARRAY_BUFFER, currentModel->buffers[MORPH_TEXCOORDS]);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)*currentModel->tBuff.size(), &targetMorph->tBuff[0]);
            }
        }
    }
}
void Entity::draw(){
    // SHADER
    //shader->use();
    if(scene->currCamera->TextureOn)
    {
        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(shader->uni.diffuseMap, 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, normalMap);
        glUniform1i(shader->uni.normalMap, 1);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, displaceMap);
        glUniform1i(shader->uni.displaceMap, 2);
    }

    glUniform4f(shader->uni.colour,
        currentModel->getMaterial0().dif[0],
        currentModel->getMaterial0().dif[1],
        currentModel->getMaterial0().dif[2],
        currentModel->getMaterial0().alpha); 

    glUniform1f(shader->uni.delta,animTimer.currAnimTime);

    /* CHANGE THIS FOR IMMEDIATE MODE*/
    if(drawMode == IMMEDIATE){
        currentModel->drawIM();
    }
    else if(drawMode == RETAINED){
        currentModel->draw();
    }
    
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    //glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_GEN_R);
    //glPopMatrix();
}
void Entity::setModelMvp(glm::mat4 & m, glm::mat4 & v, glm::mat4 & p){
    glMatrixMode(GL_MODELVIEW);
    glRotatef(zRot, 0,0,1);
    glRotatef(yRot, 0,1,0);
    glRotatef(xRot, 1,0,0);

    glTranslatef(position.x, position.y, position.z);

    shader->use();
    //shader->stop();
    glm::mat4 rot = 
            glm::rotate(glm::mat4(1.0),zRot,glm::vec3(0,0,1))*
            glm::rotate(glm::mat4(1.0),yRot,glm::vec3(0,1,0))*
            glm::rotate(glm::mat4(1.0),xRot,glm::vec3(1,0,0));

    m *= glm::vec4(scale.x,scale.y,scale.z,1.0);
    m *= rot;
    m *= glm::translate(glm::mat4(1.0),glm::vec3(position.x,position.y,position.z));

    glUniformMatrix4fv(shader->uni.model, 1, GL_FALSE,  glm::value_ptr(m));
    glUniformMatrix4fv(shader->uni.veiw, 1, GL_FALSE,   glm::value_ptr(v));
    glUniformMatrix4fv(shader->uni.proj, 1, GL_FALSE,   glm::value_ptr(p));

    glUniformMatrix4fv(shader->uni.mvp, 1, GL_FALSE,   glm::value_ptr(p*v*m));
}
void Entity::setPosition(Vec3 newPos){
    position = newPos;
}
void Entity::setPosition(float x, float y, float z){
    position.set(x,y,z);
}
void Entity::setStartPosition(Vec3 newStart){
    start_Position.set(newStart);
}
void Entity::setStartPosition(float x, float y, float z){
    start_Position.set(x,y,z);
}

void Entity::setScale(float size){
    scale.set(size,size,size);
}
void Entity::setScale(Vec3 newScale){
    scale.set(newScale);
}
void Entity::setScale(float x, float y, float z){
    scale.set(x,y,z);
}


Vec3 Entity::getPosition(){
    return position;
}
Vec3 Entity::getStartPosition(){
    return start_Position;
}
Vec3 Entity::getDirection(){
    return direction;
}
Vec3 Entity::getRight(){
    return right;
}
Vec3 Entity::getUp(){
    return up;
}