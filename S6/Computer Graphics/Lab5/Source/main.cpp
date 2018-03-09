#include <GL/glew.h>
#include <GL/glut.h>
#include "flagshader.h"

FlagShader *shader;
double x0, y0, width, height; // position and size of rectangle in pixels

bool animation = false;
int steps = 1000;
int i = 0;

void Display() 		
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    if (animation) {
        i = (i + 1) % steps;
        shader->setAnimationStage((float)i / (float)steps);
        glutPostRedisplay();
    }    

    glBegin(GL_POLYGON);
        glVertex2f(x0, y0);
        glVertex2f(x0 + width, y0);
        glVertex2f(x0 + width, y0 + height);
        glVertex2f(x0, y0 + height);
    glEnd();

    glFinish();    
}

void Reshape(GLint w, GLint h)
{
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);    /* ортографическая проекция */
    glLoadIdentity();
    glOrtho(0, w, 0, h, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    width = 0.75*w;  // rectangle for flag is 3/4 of screen in width
    height = 0.6*width;  // w / h = 1000 / 600
    x0 = (w - width) / 2.0;
    y0 = (h - height) / 2.0;
    
    shader->resize(x0, y0, 1000.0 / width);
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA);
    glutInitWindowSize(1333, 800);
    glutCreateWindow("Shaders");
    glewInit();

    shader = new FlagShader("flag.frag");
    shader->resize(100, 166, 1);
    shader->activate();

    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    glutMainLoop();

    return 0;
}

