#ifndef _TEXTUREUTILS_H
#define _TEXTUREUTILS_H

#include <string>

#include <GL/glew.h>

#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

#include <iostream>


//Store our faces
static GLubyte face[6][512][512][3];



class TextureUtils
{
public:
    static GLuint loadTexture(const std::string filename)
    {
        ILuint imageID;				// Create an image ID as a ULuint
        GLuint textureID;			// Create a texture ID as a GLuint
        ILenum error;				// Create a flag to keep track of the IL error state
        ilGenImages(1, &imageID); 	// Generate the image ID
        ilBindImage(imageID); 		// Bind the image
        GLboolean success = ilLoadImage((wchar_t*)filename.c_str()); 	// Load the image file


        if (success)
        {

            success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

            // Quit out if we failed the conversion
            if (!success)
            {
                error = ilGetError();
                printf("\nTexture creation unsuccessful: " + error);
                return NULL;
            }

            // Generate a new texture
            glGenTextures(1, &textureID);

            // Bind the texture to a name
            glBindTexture(GL_TEXTURE_2D, textureID);

            // Set Environment
            glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
            //glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

            // Set texture clamping method
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            // Set texture interpolation method to use linear interpolation (no MIPMAPS)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

            // Specify the texture specification
            glTexImage2D(GL_TEXTURE_2D, 				// Type of texture
                0,				                // Pyramid level (for mip-mapping) - 0 is the top level
                ilGetInteger(IL_IMAGE_BPP),	// Image colour depth
                ilGetInteger(IL_IMAGE_WIDTH),	// Image width
                ilGetInteger(IL_IMAGE_HEIGHT),	// Image height
                0,				                // Border width in pixels (can either be 1 or 0)
                ilGetInteger(IL_IMAGE_FORMAT),	// Image format (i.e. RGB, RGBA, BGR etc.)
                GL_UNSIGNED_BYTE,		        // Image data type
                ilGetData());			        // The actual image data itself
        }
        else // If we failed to open the image file in the first place...
        {
            error = ilGetError();
            printf("\nImage load failed - IL reports error: %i - %s", error, iluErrorString(error));
            return NULL;
        }

        ilDeleteImages(1, &imageID);    // Because we have already copied image data into texture data we can release memory used by image.

        std::cout << "\nTexture creation successful.";

        GLenum c = glGetError();
        if(c != GL_NO_ERROR){
            std::cout<<gluErrorString(c);
        }

        return textureID;               // Return the GLuint to the texture so you can use it!
    }
    static GLuint loadCubeEnvMap(std::string filename)
    {
        ILuint imageID = 1;			// Create an image ID as a ULuint
        GLuint textureID;			// Create a texture ID as a GLuint
        ILenum error;				// Create a flag to keep track of the IL error state
        ilGenImages(1, &imageID); 	// Generate the image ID
        ilBindImage(imageID); 		// Bind the image
        GLboolean success;       	// Load the image file



        for (int faceNum = 0; faceNum < 6; faceNum++)
        {
            std::string mapName;
            switch(faceNum){
            case 0:
                mapName = filename+"_positive_x.jpg";
                break;
            case 1:
                mapName = filename+"_negative_x.jpg";
                break;
            case 2:
                mapName = filename+"_positive_y.jpg";
                break;
            case 3:
                mapName = filename+"_negative_x.jpg";
                break;
            case 4:
                mapName = filename+"_positive_z.jpg";
                break;
            case 5:
                mapName = filename+"_negative_z.jpg";
                break;
            }
            success = ilLoadImage((wchar_t*)mapName.c_str());

            if (success)
            {
                success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);

                // Quit out if we failed the conversion
                if (!success)
                {
                    error = ilGetError();
                    printf("\nTexture creation unsuccessful: " + error);
                    return NULL;
                }

                //Start assigning our RGB values to each face
                int h = ilGetInteger(IL_IMAGE_HEIGHT);
                int w = ilGetInteger(IL_IMAGE_WIDTH);

                for (int row=0; row< h;row++)
                {
                    for (int column=0; column < w; column++)
                    {
                        ilCopyPixels(column, row, 0, 1,1,1, IL_RGB, IL_UNSIGNED_BYTE, face[faceNum][row][column]);
                        //face[faceNum][row][column][1]= img_data.GetPixel(column, row).g;
                        //face[faceNum][row][column][2]= img_data.GetPixel(column, row).b;
                    }

                }

            }
            else // If we failed to open the image file in the first place...
            {
                error = ilGetError();
                printf("\nImage load failed - IL reports error: %i - %s", error, iluErrorString(error));
                return NULL;
            }
        }
        // Generate a new texture
        glGenTextures(1, &textureID);

        // Bind the texture to a name
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        for (int i=0; i<6; i++)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,  				// Type of texture
                0,				                // Pyramid level (for mip-mapping) - 0 is the top level
                ilGetInteger(IL_IMAGE_BPP),	    // Image colour depth
                ilGetInteger(IL_IMAGE_WIDTH),	// Image width
                ilGetInteger(IL_IMAGE_HEIGHT),	// Image height
                0,				                // Border width in pixels (can either be 1 or 0)
                IL_RGB,	                        // Image format (i.e. RGB, RGBA, BGR etc.)
                GL_UNSIGNED_BYTE,		        // Image data type
                &face[i][0][0][0]);			    // The actual image data itself
        }
        GLenum c = glGetError();
        if(c != GL_NO_ERROR){
            std::cout<<gluErrorString(c);
        }
        glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );

        // Set texture interpolation method to use linear interpolation (no MIPMAPS)
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
        glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);

        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
        glEnable(GL_TEXTURE_GEN_R);

        ilDeleteImages(1, &imageID);    // Because we have already copied image data into texture data we can release memory used by image.

        std::cout << "\nTexture creation successful.";
        c = glGetError();
        if(c != GL_NO_ERROR){
            std::cout<<gluErrorString(c);
        }
        return textureID;               // Return the GLuint to the texture so you can use it!

    }
    static GLuint loadSphereEnvMap(const std::string filename){
        ILuint imageID;				// Create an image ID as a ULuint
        GLuint textureID;			// Create a texture ID as a GLuint
        ILenum error;				// Create a flag to keep track of the IL error state
        ilGenImages(1, &imageID); 	// Generate the image ID
        ilBindImage(imageID); 		// Bind the image
        GLboolean success = ilLoadImage((wchar_t*)filename.c_str()); 	// Load the image file


        if (success)
        {

            success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

            // Quit out if we failed the conversion
            if (!success)
            {
                error = ilGetError();
                printf("\nTexture creation unsuccessful: " + error);
                return NULL;
            }

            // Generate a new texture
            glGenTextures(1, &textureID);

            // Bind the texture to a name
            glBindTexture(GL_TEXTURE_2D, textureID);

            // Set Environment
            //glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
            glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

            // Set texture clamping method
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

            // Set texture interpolation method to use linear interpolation (no MIPMAPS)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

            // Specify the texture specification
            glTexImage2D(GL_TEXTURE_2D, 				// Type of texture
                0,				                // Pyramid level (for mip-mapping) - 0 is the top level
                ilGetInteger(IL_IMAGE_BPP),	// Image colour depth
                ilGetInteger(IL_IMAGE_WIDTH),	// Image width
                ilGetInteger(IL_IMAGE_HEIGHT),	// Image height
                0,				                // Border width in pixels (can either be 1 or 0)
                ilGetInteger(IL_IMAGE_FORMAT),	// Image format (i.e. RGB, RGBA, BGR etc.)
                GL_UNSIGNED_BYTE,		        // Image data type
                ilGetData());			        // The actual image data itself
            GLenum c = glGetError();
            if(c != GL_NO_ERROR){
                std::cout<<gluErrorString(c);
            }
            //Specific for sphere mapping
            glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
            glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

            //Enable the tex coords
            glEnable(GL_TEXTURE_GEN_S);
            glEnable(GL_TEXTURE_GEN_T);
        }
        else // If we failed to open the image file in the first place...
        {
            error = ilGetError();
            printf("\nImage load failed - IL reports error: %i - %s", error, iluErrorString(error));
            return NULL;
        }

        ilDeleteImages(1, &imageID);    // Because we have already copied image data into texture data we can release memory used by image.

        std::cout << "\nTexture creation successful.";

        return textureID;               // Return the GLuint to the texture so you can use it!
    }
    
   // static FBO * createFBO(bool colour, bool depth, int width, int height){
   //     if(!colour && !depth){
   //         return 0;
   //     }
   //     
   //     GLuint fboHandle;
   //     GLuint depthBuffer;
   //     GLuint colourTexture;

   //     glGenFramebuffers(1, &fboHandle);
   //     glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);

   //     if(colour){
   //         glGenTextures(1, &colourTexture);
   //         glBindTexture(GL_TEXTURE_2D, colourTexture);

   //         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
   //         
   //         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   //         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   //         //glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
			//
   //         glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
			//	GL_TEXTURE_2D, colourTexture, 0);
   //     }

   //     if(depth){
   //         glGenTextures(1, &depthBuffer);
		 //   glBindTexture(GL_TEXTURE_2D, depthBuffer);
		 //   glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, 
			//    GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, 0);
		 //   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		 //   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		 //   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		 //   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		 //   glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 
			//    GL_TEXTURE_2D, depthBuffer, 0);
   //     }

   //     GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
   //     if(status != GL_FRAMEBUFFER_COMPLETE){
   //         printf("\nFrame Buffer Creation failed!");
   //         return 0;
   //     }
   //     
   //     glBindTexture(GL_TEXTURE_2D, 0);
   //     glBindFramebuffer(GL_FRAMEBUFFER, 0);

   //     FBO * result = new FBO(fboHandle,colourTexture,depthBuffer);
   //     result->x = width;
   //     result->y = height;
   //     return result;
   // }
    static GLuint createFBO(bool colour, bool depth, int width, int height,
        GLuint *depthBuffer, GLuint *colourTexture){
        if(!colour && !depth){
            return -1;
        }

        GLuint fboHandle;

        glGenFramebuffers(1, &fboHandle);
        glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);

        if(colour){
            glGenTextures(1, colourTexture);
            glBindTexture(GL_TEXTURE_2D, *colourTexture);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            //glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
			
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
				GL_TEXTURE_2D, *colourTexture, 0);
        }

        if(depth){
            glGenTextures(1, depthBuffer);
		    glBindTexture(GL_TEXTURE_2D, *depthBuffer);
		    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, 
			    GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, 0);
		    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 
			    GL_TEXTURE_2D, *depthBuffer, 0);
        }

        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if(status != GL_FRAMEBUFFER_COMPLETE){
            printf("\nFrame Buffer Creation failed!");
            return -1;
        }
        
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        return fboHandle;
    }
};

struct FBO{
    FBO(GLuint _fbo, GLuint _colour, GLuint _depth){
        fbo     = _fbo;
        colour  = _colour;
        depth   = _depth;
    }
    FBO(){
        fbo     = -1;
        colour  = -1;
        depth   = -1;

    }
    void create(bool _colour, bool _depth, int _x, int _y){
        x = _x; y = _y;
        fbo = TextureUtils::createFBO(_colour, _depth, x, y, &depth, &colour);
    }
    GLuint fbo;
    GLuint depth;
    GLuint colour;

    int x,y;
};

#endif