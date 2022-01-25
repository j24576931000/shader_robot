#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <GL\glut.h>
#include <glm/glm.hpp>


class Texture
{
    private:
        
        GLuint id;
        int image_width;
        int image_height;
        unsigned int type;
        //GLuint textureUnit;

    public:
        Texture(const char* filename,GLenum type)
        //Texture(const char* filename,GLenum type,/*GLuint texture_unit*/)
        {
            this->type=type;
            //this->textureUnit=texture_unit;
        
            int imgcor;
            stbi_set_flip_vertically_on_load(true);
           
            unsigned char* bytes =stbi_load(filename,&this->image_width,&this->image_height,&imgcor,0);
            
            glGenTextures(1, &this->id);
            
            
            
            
            glBindTexture(type, this->id);
            glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glGenerateMipmap(type);
            
            glTexImage2D(type, 0, GL_RGB, this->image_width,this->image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE,bytes);
            glActiveTexture(0);
            glBindTexture(type, 0);
            
            stbi_image_free(bytes);
        }
        ~Texture()
        {
            glDeleteTextures(1, &this->id);
        }
        inline GLuint getID() const{return this->id;}

        void bind(const GLuint textureUnit)
        {
            glActiveTexture(GL_TEXTURE0 +textureUnit/*+ this->textureUnit*/);
            glBindTexture(this->type, this->id);
        }
        void unbind()
        {
            glActiveTexture(0);
            glBindTexture(this->type, 0);
        }
        //GLint getTextureUnit() const { return this->textureUnit; }

        

        
        /*void bind(GLuint texture_unit,GLenum type)
        {
            glActiveTexture(GL_TEXTURE0 + texture_unit);
            glBindTexture(type, this->id);
        }
        void unbind(GLenum type)
        {
            glActiveTexture(0);
            glBindTexture(type, 0);
        }*/
        

};