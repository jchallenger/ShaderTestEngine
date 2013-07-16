#include "Model.h"
#include "Entity.h"

Model::Model(void)
{
    name = "NULL";
}
Model::Model(std::string file)
{
    load(file);
    name = file;
    morphing = false;
}

Model::~Model(void)
{
}

Material::Material(const char* _name, float _al, float _ns, float * _di, float * _am, float *_sp, int ill){
    name=name;      // Name of Material
    alpha=_al;      // Aplha
    ns = _ns;       // Specular Highlight

    dif[0]=_di[0];  // Diffuse Color (RGB)
    dif[1]=_di[1];
    dif[2]=_di[2];

    amb[0]=_am[0];  // Ambience Colour (RGB)
    amb[1]=_am[1];
    amb[2]=_am[2];

    spec[0]=_sp[0]; // Specular Colour (RGB)
    spec[1]=_sp[1];
    spec[2]=_sp[2];

    illum=ill;      // Illum level (detail?)
}
Material::Material(){
    name="Blank";      // Name of Material
    alpha=1;      // Aplha
    ns = 60;       // Specular Highlight

    dif[0]=0.5f;  // Diffuse Color (RGB)
    dif[1]=0.5f;
    dif[2]=0.5f;

    amb[0]=0;  // Ambience Colour (RGB)
    amb[1]=0;
    amb[2]=0;

    spec[0]=0; // Specular Colour (RGB)
    spec[1]=0;
    spec[2]=0;

    illum=4;      // Illum level (detail?)
}

bool Model::load(std::string filename){
    std::ifstream read(filename);
    std::string c;
    if(read.fail()){
        std::cerr << "\nCant Open OBJ";
        return 0;
    }
    name = filename;
    xyz temp(0,0,0);
    UVmap uvTemp(0,0);
    Face readFace;
    int f_index = 0, findMat = 0, curMat = -1;
    hasMaterial = false;
    char tempChar;

    while(!read.eof()){
        if(f_index == 0)
            read >> c;
        if(c == "f" || f_index > 0){
            read >> readFace.v_ind[f_index];
            readFace.v_ind[f_index]--;
            read.get(tempChar);
            read >> readFace.t_ind[f_index];
            readFace.t_ind[f_index]--;
            read.get(tempChar);
            read >> readFace.n_ind[f_index];
            readFace.n_ind[f_index]--;
            read.get(tempChar);
            
            f_index++;
            if(f_index > 2){
                f_index = 0;
                readFace.mat = curMat;
                faces.push_back(readFace);
            }
            continue;
        }
        
        if(c == "mtllib"){
            read >> c;
            hasMaterial = loadMats(c);
        }
        else if(c == "usemtl" && hasMaterial)
        {
            read >> c;
            for(findMat = 0; materials[findMat].name.compare(c) && findMat != materials.size()-1; findMat++){
                // Do Nothing: This Matches material indexes based on name
            }
            curMat = findMat;
        }
        

        else if(c == "v"){
            read >> temp.x;
            read >> temp.y;
            read >> temp.z;
            read.get();
            verts.push_back(temp);
        }
        else if(c == "vn"){
            read >> temp.x;
            read >> temp.y;
            read >> temp.z;
            read.get();
            norms.push_back(temp);
        }
        else if(c == "vt"){
            read >> uvTemp.u;
            read >> uvTemp.v;
            read.get();
            UVs.push_back(uvTemp);
        }
        else{
            read.ignore(500,'\n');
        }
        
    }
    if(parentEntity != NULL){
        if(parentEntity->texture == -1){
            parentEntity->texture = TextureUtils::loadTexture("Resources/Images/missing.png");
        }
    }
    return 1;
}
void Model::bind(Shader * shader){
    // Create the VAO
    glGenVertexArrays(1, &vao); 
    glBindVertexArray(vao);

    // Create the buffers for the vertices atttributes
    glGenBuffers(7, buffers);

    // Vertices
    short size = verts.size();
    if(size == -1 || size == NULL) return;
    
    numOfVerts  = verts.size()  *3;
    numOfNorms  = norms.size()  *3;
    numOfUVs    = UVs.size()    *2;

    for(unsigned int f = 0; f < faces.size(); f++){
        for(int v = 0; v < 3; v++){
            if(faces[f].v_ind[v] < 0){
                std::cout << "\nThis OBJ has too many vertices for UNSIGNED_SHORT";
                getchar();
            }
            vBuff.push_back(verts[faces[f].v_ind[v]].x);
            vBuff.push_back(verts[faces[f].v_ind[v]].y);
            vBuff.push_back(verts[faces[f].v_ind[v]].z);
            
            tBuff.push_back(UVs[faces[f].t_ind[v]].u);
            tBuff.push_back(1-UVs[faces[f].t_ind[v]].v);
            
            nBuff.push_back(norms[faces[f].n_ind[v]].x);
            nBuff.push_back(norms[faces[f].n_ind[v]].y);
            nBuff.push_back(norms[faces[f].n_ind[v]].z);
        }
    }
    //// Vertices
    drawArraySize = 0;
    if(shader->attr.coord3d != -1){
        drawArraySize += numOfVerts;
        glBindBuffer(GL_ARRAY_BUFFER, buffers[VERTICES]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vBuff.size(), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)*vBuff.size(), &vBuff[0]);
        //glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vBuff.size(), &vBuff[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(shader->attr.coord3d);
        glVertexAttribPointer(shader->attr.coord3d, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }
    //// Normals
    if(shader->attr.normals != -1){
        drawArraySize += numOfNorms;
        glBindBuffer(GL_ARRAY_BUFFER, buffers[NORMALS]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*nBuff.size(), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)*nBuff.size(), &nBuff[0]);
        //glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*nBuff.size(), &nBuff[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(shader->attr.normals);
        glVertexAttribPointer(shader->attr.normals, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }
    //// Textures
    if(shader->attr.texcoord != -1){
        drawArraySize += numOfUVs;
        glBindBuffer(GL_ARRAY_BUFFER, buffers[TEXCOORDS]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*tBuff.size(), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)*tBuff.size(), &tBuff[0]);
        //glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*tBuff.size(), &tBuff[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(shader->attr.texcoord);
        glVertexAttribPointer(shader->attr.texcoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
    }
    
    //// Morph Vertices
    if(shader->attr.coord3d2 != -1){
        drawArraySize += numOfVerts;
        glBindBuffer(GL_ARRAY_BUFFER, buffers[MORPH_VERTICES]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vBuff.size(), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)*vBuff.size(), &vBuff[0]);
        glEnableVertexAttribArray(shader->attr.coord3d2);
        glVertexAttribPointer(shader->attr.coord3d2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }
    //// Normals
    if(shader->attr.normals2 != -1){
        drawArraySize += numOfNorms;
        glBindBuffer(GL_ARRAY_BUFFER, buffers[MORPH_NORMALS]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*nBuff.size(), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)*nBuff.size(), &nBuff[0]);
        glEnableVertexAttribArray(shader->attr.normals2);
        glVertexAttribPointer(shader->attr.normals2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }
    //// Textures
    if(shader->attr.texcoord2 != -1){
        drawArraySize += numOfUVs;
        glBindBuffer(GL_ARRAY_BUFFER, buffers[MORPH_TEXCOORDS]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*tBuff.size(), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)*tBuff.size(), &tBuff[0]);
        glEnableVertexAttribArray(shader->attr.texcoord2);
        glVertexAttribPointer(shader->attr.texcoord2, 2, GL_FLOAT, GL_FALSE, 0, 0);
    }
    

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[INDEX]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces.data()), &faces.data()[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
bool Model::loadMats(std::string filename){
    char _na[256];
    float alp = 1, _ns=1;
    float _di[3], _am[3], _sp[3];
    int ill;
    //GLuint tex;
    bool first = true;
    char temp[256];

    std::ifstream mtLib(filename);
    if(mtLib.fail()){
        mtLib.open("Resources/Models/"+filename);
        if(mtLib.fail()){
		    std::cerr << "\nFailed to open " << filename;
            return 0;
        }
    }
    while(mtLib >> temp)
    {
        if(!strcmp(temp,"newmtl"))
        {
            //Saves current info
            if(!first){
                materials.push_back(Material(_na,alp, _ns,_di,_am,_sp,ill));
            }
            else
                first = false;

            alp = _ns = 1.0f;
            _di[0] = 0.5f;
            _di[1] = 0.5f;
            _di[2] = 0.5f;
            _am[0] = 0.0f;
            _am[1] = 0.0f;
            _am[2] = 0.0f;
            _sp[0] = 0.0f;
            _sp[1] = 0.0f;
            _sp[2] = 0.0f;

            ill = 1;

            mtLib.ignore(1);
            mtLib.getline(_na,256);
        }
        else if(!strcmp(temp,"illum")){
            // Illumination Model #
            mtLib >> ill;
        }
        else if(!strcmp(temp,"Kd")){
            //Diffuse
            mtLib >> 
                _di[0] >> 
                _di[1] >> 
                _di[2];
        }
        else if(!strcmp(temp,"Ka")){
            // Ambient Reflectivity
            mtLib >> 
                _am[0] >> 
                _am[1] >>
                _am[2];
        }
        else if(!strcmp(temp,"Map_Kd")){
            mtLib >> temp;
            if(parentEntity != NULL)
                parentEntity->texture = TextureUtils::loadTexture("Resources/Images/"+*temp);
        }
        else if(!strcmp(temp,"Ks")){
            // Spectral Reflectivity
            mtLib >> 
                _sp[0] >> 
                _sp[1] >> 
                _sp[2];
        }
        
        else if(!strcmp(temp,"Ns")){
            // Specular Highlight
            mtLib >> _ns;
        }
        else
        {
            mtLib.ignore(512,'\n');
            continue;
        }
    }
    materials.push_back(Material(_na,alp, _ns,_di,_am,_sp,ill));
    return 1;
}
void Model::drawWire(){
    glLineWidth(1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glGetError();
    
    //glPushMatrix();
    for(int x = 0; x < faces.size(); x++){
    glBegin(GL_POLYGON);

        for(int i = 0; i < 3; i++){
            if(hasMaterial && faces[x].mat != -1){
                glColor3f(
                    materials[faces[x].mat].dif[0]-0.2f,
                    materials[faces[x].mat].dif[1]+0.2f,
                    materials[faces[x].mat].dif[2]-0.2f);
            }
            else{
                glColor3f(0.5,0.5,0.5);
            }
            glVertex3f( 
                (verts[faces[x].v_ind[i]].x),
                (verts[faces[x].v_ind[i]].y),
                (verts[faces[x].v_ind[i]].z));
        }
	    glEnd();
    }
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //glPopMatrix();
    return;
}
void Model::drawIM(){
    
    //glUseProgram(0);
    //glDisable(GL_LIGHTING);
    errC = glGetError();
    if(errC != GL_NO_ERROR){
        std::cout << gluErrorString(errC) << std::endl;
    }

    glPushMatrix();
    for(unsigned int x = 0; x < faces.size(); x++)
    {
        
        glBegin(GL_TRIANGLES);
        for(int i = 0; i < 3; i++){
            glNormal3f(
                norms[faces[x].n_ind[i]].x,
                norms[faces[x].n_ind[i]].y,
                norms[faces[x].n_ind[i]].z);
            glVertex3f( 
                verts[faces[x].v_ind[i]].x,
                verts[faces[x].v_ind[i]].y,
                verts[faces[x].v_ind[i]].z);
            glTexCoord2f( 
                UVs[faces[x].v_ind[i]].u,
                1-UVs[faces[x].v_ind[i]].v);
        }
	    glEnd();
        errC = glGetError();
        if(errC != GL_NO_ERROR){
            std::cout << "Model.cpp  Line 375: " << gluErrorString(errC) << std::endl;
        }        
    }
    glPopMatrix();
    return;
}
void Model::draw(){
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, drawArraySize);
    glBindVertexArray(0);    
}

Material Model::getMaterial0(){
    if(this->hasMaterial){
        return materials[0];
    }
    return Material();
}