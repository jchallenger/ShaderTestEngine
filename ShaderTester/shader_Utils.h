#ifndef _SHADER_H
#define _SHADER_H

#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "StringUtils.h"

namespace ShaderLight{
    static const int MAX_LIGHTS = 8;
}
enum vbo{
    VERTICES = 0,
    NORMALS,
    TEXCOORDS,
    MORPH_VERTICES,
    MORPH_NORMALS,
    MORPH_TEXCOORDS,
    INDEX
};

struct Uni_Light{
    GLint position, ambience, diffuse, attenuation, specular;
};
typedef struct Attributes{
    GLint coord3d, coord3d2, texcoord, texcoord2, normals, normals2;
}_Attributes;
typedef struct Uniforms{
    GLint eyePosition, rotation, scale, translate,  model, veiw, proj, mvp, colour, delta, numLightsActive;
    GLint diffuseMap, normalMap, displaceMap, shadowMap;

    Uni_Light light;
    Uni_Light lights[ShaderLight::MAX_LIGHTS];
}_Uniforms;


class Shader{
public:
    Shader(void);
    Shader(std::string filename);
    Shader(std::string vertex, std::string fragment);
    ~Shader();

    bool load(const std::string vertex, const std::string fragment);
    bool createFromString(char * Vert, char * Frag);
    void setName(std::string name);
    void bind();
    void use();
    void stop();
    
    bool checkBind(const char * name, int location, bool stopIfMissing = false);

    Uniforms uni;
    Attributes attr;

    bool hasRotation(){     return uni.rotation     != -1 ? true : false; }
    bool hasScale(){        return uni.scale        != -1 ? true : false; }
    bool hasTranslate(){    return uni.translate    != -1 ? true : false; }
    bool hasTexture(){      return uni.diffuseMap   != -1 ? true : false; }
    bool hasColour(){       return uni.colour       != -1 ? true : false; }

    GLuint id(){ return program; }

private:
    GLuint program;
    
    std::string shaderName;

    char* file_read(const char* filename);
    void print_log(GLuint object);
    GLuint create_shader(const char* filename, GLenum type);
};


#endif
