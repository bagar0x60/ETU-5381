#ifndef FLAGSHADER_H
#define FLAGSHADER_H

#include <GL/glew.h>
#include <GL/glut.h>

class FlagShader {
private:
    double x0 = 0, y0 = 0;
    double stretch = 1;

    GLint programID = -1;

    char* textFileRead(char *fn);
    GLuint setShaders(char* shaderFileName);
    
public:
    FlagShader(char* shaderFileName);
    
    void resize(float x0, float y0, float stretch);
    void activate();
    void deactivate();  
    void setAnimationStage(float t);  
};

#endif // FLAGSHADER_H