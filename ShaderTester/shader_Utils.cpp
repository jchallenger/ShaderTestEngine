#include "shader_Utils.h"

// Loads BasicTex.v and .f GLSL
Shader::Shader(void)
{
    /*load("basicTex.v.glsl", "basicTex.f.glsl");
    bind();*/
    //use();
}
// Without .x.glsl
Shader::Shader(std::string filename)    
{
    if(!load(filename+".v.glsl", filename+".f.glsl"))
        load("basicTex.v.glsl", "basicTex.f.glsl");
    bind();
    //use();
}
Shader::Shader(std::string vertex, std::string fragment)
{
    if(!load(vertex+".v.glsl", fragment+".f.glsl"))
        load("basicTex.v.glsl", "basicTex.f.glsl");
    bind();
    //use();
}
Shader::~Shader(){
    glDeleteProgram(program);
}
void Shader::setName(std::string name){
    shaderName = name;
}
char* Shader::file_read(const char* filename){
    FILE* input;
    fopen_s(&input, filename, "rb");

    if(input == NULL)                   return NULL;
    if(fseek(input,0,SEEK_END) == -1)   return NULL;

    long size = ftell(input);

    if(size == -1)                      return NULL;
    if(fseek(input,0,SEEK_SET) == -1)   return NULL;
    
    char *content = (char*)malloc((size_t)size+1);

    if(content == NULL)                 return NULL;

    fread(content,1,(size_t)size,input);
    if(ferror(input)){
        free(content);
        return NULL;
    }

    fclose(input);
    content[size]='\0';
    return content;
}
GLuint Shader::create_shader(const char* filename, GLenum type)
{
    const GLchar* source = file_read(filename);
    if (source == NULL) {
		printf("\nError opening %s: ",filename);
		perror("");
        return 0;
    }
    GLuint res = glCreateShader(type);
//    const GLchar* sources[] = {
//#ifdef GL_ES_VERSION_2_0
//        "#version 100\n"
//        "#define GLES2\n"
//        // Define default float precision for fragment shaders:
//        (type == GL_FRAGMENT_SHADER) ?
//        "#ifdef GL_FRAGMENT_PRECISION_HIGH\n"
//        "precision highp float;           \n"
//        "#else                            \n"
//        "precision mediump float;         \n"
//        "#endif                           \n"
//        : ""
//        // Note: OpenGL ES automatically defines this:
//        // #define GL_ES
//#else
//        "#version 420\n"
//        // Ignore GLES 2 precision specifiers:
//        "#define lowp   \n"
//        "#define mediump\n"
//        "#define highp  \n"
//#endif
//        ,
//        source };
    glShaderSource(res, 1, &source, NULL);
    free((void*)source);

    glCompileShader(res);
    GLint compile_ok = GL_FALSE;
    glGetShaderiv(res, GL_COMPILE_STATUS, &compile_ok);
    if (compile_ok == GL_FALSE) {
		printf("\n%s:",filename);
        print_log(res);
        glDeleteShader(res);
        return 0;
    }

    return res;
}
void Shader::print_log(GLuint object){
    GLint log_length = 0;
    if(glIsShader(object) || glIsProgram(object))
        glGetShaderiv(object,GL_INFO_LOG_LENGTH, &log_length);
    else{
		printf("\nprintlog: Not a shader or a Program");
        return;
    }

    char* log = (char*)malloc(log_length);
 
    if (glIsShader(object) || glIsProgram(object))
        glGetShaderInfoLog(object, log_length, NULL, log);

	printf("\n%s",log);
    free(log);
}
bool Shader::createFromString(char * Vert, char * Frag){
    /**********************************
    ***********LOAD SHADERS************
    **********************************/
    GLint link_ok = GL_FALSE;
    GLuint vs, fs;
    //if ((vs = create_shader(Vert, GL_VERTEX_SHADER))   == 0) link_ok = 0;
    //if ((fs = create_shader(Frag, GL_FRAGMENT_SHADER)) == 0) link_ok = 0;

    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    
    glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
    if (!link_ok) {
		printf("\nglLinkProgram:");
        print_log(program);
        return false;
    }
    return true;
}
bool Shader::load(const std::string vertex, const std::string fragment){
    
    printf("\n--------------------------------------------------------------------");
    printf("\nLoading Shader: \n    %s, \n    %s", vertex.c_str(), fragment.c_str());
    
    /**********************************
    ***********LOAD SHADERS************
    **********************************/
    GLint link_ok = GL_FALSE;
    GLuint vs, fs;
    if ((vs = create_shader((char*)vertex.c_str(), GL_VERTEX_SHADER))   == 0) link_ok = 0;
    if ((fs = create_shader((char*)fragment.c_str(), GL_FRAGMENT_SHADER)) == 0) link_ok = 0;

    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    
    glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
    if (!link_ok) {
		printf("\nglLinkProgram:");
        print_log(program);
        return false;
    }
    return true;
}
bool Shader::checkBind(const char * name, int location, bool stopIfMissing){
    if (location != -1) {
		printf("\nBound location %i: \t%s",location, name);
        return true;
    }
    else if(stopIfMissing){
        printf("\nFailed to bind input: \t%s", name);
        getchar();
    }
    return false;
}
void Shader::bind(){
    /**********************************
    *******BIND SHADER ATTRIBUTES******
    **********************************/
    printf("\n--Binding Attributes--");
    // Coordinates
    char* attribute_name = "coord3d";
    attr.coord3d = glGetAttribLocation(program, attribute_name);
    checkBind(attribute_name, attr.coord3d, false);
    // Normals
    attribute_name = "normals";
    attr.normals = glGetAttribLocation(program, attribute_name);
    checkBind(attribute_name, attr.normals);
    // TexCoords
    attribute_name = "texcoord";
    attr.texcoord = glGetAttribLocation(program, attribute_name);
    checkBind(attribute_name, attr.texcoord);
    // MORPHING
    // Coordinates
    attribute_name = "coord3d_2";
    attr.coord3d2 = glGetAttribLocation(program, attribute_name);
    checkBind(attribute_name, attr.coord3d2);
    // Normals
    attribute_name = "normals_2";
    attr.normals2 = glGetAttribLocation(program, attribute_name);
    checkBind(attribute_name, attr.normals2);
    // TexCoords
    attribute_name = "texcoord_2";
    attr.texcoord2 = glGetAttribLocation(program, attribute_name);
    checkBind(attribute_name, attr.texcoord2);

    /**********************************
    ***********BIND UNIFORMS***********
    **********************************/
    printf("\n--Binding Uniforms--");
    // Transform Matrix
    attribute_name = "m";
    uni.model = glGetUniformLocation(program, attribute_name);
    checkBind(attribute_name, uni.model, false);
    // Transform Matrix
    attribute_name = "v";
    uni.veiw = glGetUniformLocation(program, attribute_name);
    checkBind(attribute_name, uni.veiw, false);
    // Transform Matrix
    attribute_name = "p";
    uni.proj = glGetUniformLocation(program, attribute_name);
    checkBind(attribute_name, uni.proj, false);
    // Transform Matrix
    attribute_name = "mvp";
    uni.mvp = glGetUniformLocation(program, attribute_name);
    checkBind(attribute_name, uni.mvp, false);

    // Position
    attribute_name = "translate";
    uni.translate = glGetUniformLocation(program, attribute_name);
    checkBind(attribute_name, uni.translate);
    // Scale
    attribute_name = "scale";
    uni.scale = glGetUniformLocation(program, attribute_name);
    checkBind(attribute_name, uni.scale);
    // Rotation
    attribute_name = "rotation";
    uni.rotation = glGetUniformLocation(program, attribute_name);
    checkBind(attribute_name, uni.rotation);
    // Colour
    attribute_name = "colour";
    uni.colour = glGetUniformLocation(program, attribute_name);
    checkBind(attribute_name, uni.colour);

    // Texture
    //*********************************************************************************************
    // Diffuse Texture
    attribute_name = "diffuseMap";
    uni.diffuseMap = glGetUniformLocation(program, attribute_name);
    checkBind(attribute_name, uni.diffuseMap);
    // Normal Map
    attribute_name = "normalMap";
    uni.normalMap = glGetUniformLocation(program, attribute_name);
    checkBind(attribute_name, uni.normalMap);
    // Displacement Map
    attribute_name = "displaceMap";
    uni.displaceMap = glGetUniformLocation(program, attribute_name);
    checkBind(attribute_name, uni.displaceMap);
    // Shadow Map
    attribute_name = "shadowMap";
    uni.shadowMap = glGetUniformLocation(program, attribute_name);
    checkBind(attribute_name, uni.shadowMap);

    // Time
    attribute_name = "delta";
    uni.delta = glGetUniformLocation(program, attribute_name);
    checkBind(attribute_name, uni.delta);

    // Eye Position
    attribute_name = "eyePos";
    uni.eyePosition = glGetUniformLocation(program, attribute_name);
    checkBind(attribute_name, uni.eyePosition);
    

    // LIGHT
    attribute_name = "light_pos";
    uni.light.position = glGetUniformLocation(id(), attribute_name);
    checkBind(attribute_name, uni.light.position);
    attribute_name = "light_dif";
    uni.light.diffuse = glGetUniformLocation(id(), attribute_name);
    checkBind(attribute_name, uni.light.diffuse);
    attribute_name = "light_amb";
    uni.light.ambience = glGetUniformLocation(id(), attribute_name);
    checkBind(attribute_name, uni.light.ambience);
    attribute_name = "light_spec";
    uni.light.specular = glGetUniformLocation(id(), attribute_name);
    checkBind(attribute_name, uni.light.specular);

    
    attribute_name = "LIGHTS_ACTIVE";
    uni.numLightsActive = glGetUniformLocation(id(), attribute_name);
    checkBind(attribute_name, uni.numLightsActive);
    
    for(int i = 0; i < ShaderLight::MAX_LIGHTS; i++){
        string name = "Lights[" + convertIntToString(i) + "].position";
        uni.lights[i].position = glGetUniformLocation(id(), name.c_str());
        checkBind(name.c_str(), uni.lights[i].position);
        name = "Lights[" + convertIntToString(i) + "].diffuse";
        uni.lights[i].diffuse = glGetUniformLocation(id(), name.c_str());
        checkBind(name.c_str(), uni.lights[i].diffuse);
        name = "Lights[" + convertIntToString(i) + "].specular";
        uni.lights[i].specular = glGetUniformLocation(id(), name.c_str());
        checkBind(name.c_str(), uni.lights[i].specular);
        name = "Lights[" + convertIntToString(i) + "].ambience";
        uni.lights[i].ambience = glGetUniformLocation(id(), name.c_str());
        checkBind(name.c_str(), uni.lights[i].ambience);
    }

}
void Shader::use(){
    glUseProgram(program);
}
void Shader::stop(){
    glUseProgram(0);
}
