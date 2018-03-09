#include "flagshader.h"
#include <stdlib.h>
#include <stdio.h>

FlagShader::FlagShader(char* shaderFileName) {
    programID = setShaders(shaderFileName);
    setAnimationStage(1.0);
}

char* FlagShader::textFileRead(char* fn) {
 
    FILE *fp;
    char *content = NULL;
 
    int count=0;
 
    if (fn != NULL) {
        fp = fopen(fn,"rt");
 
        if (fp != NULL) {
 
      fseek(fp, 0, SEEK_END);
      count = ftell(fp);
      rewind(fp);
 
            if (count > 0) {
                content = (char *)malloc(sizeof(char) * (count+1));
                count = fread(content,sizeof(char),count,fp);
                content[count] = '\0';
            }
            fclose(fp);
        }
    }
    return content;
}

GLuint FlagShader::setShaders(char* shaderFileName) {
    GLuint f;
    char *fs;

    f = glCreateShader(GL_FRAGMENT_SHADER);

    // Read source code from files
    fs = textFileRead(shaderFileName);

    const char * ff = fs;

    // Set shader source
    glShaderSource(f, 1, &ff, NULL);

    free(fs);

    // Compile all shaders
    glCompileShader(f);


    // Create the program
    GLuint p = glCreateProgram();

    // Attach shaders to program
    glAttachShader(p, f);

    // Link and set program to use
    glLinkProgram(p);

    return p;
}

void FlagShader::resize(float x0, float y0, float stretch) {
    GLint location = glGetUniformLocation(programID, "rectanglePosition");
    glProgramUniform2f(programID, location, x0, y0);

    location = glGetUniformLocation(programID, "stretch");
    glProgramUniform1f(programID, location, stretch);
}

void FlagShader::activate() {
    glUseProgram(programID);
}

void FlagShader::deactivate() {
    // do something
}

void FlagShader::setAnimationStage(float t) {
    GLint location = glGetUniformLocation(programID, "t");
    glProgramUniform1f(programID, location, t);
}