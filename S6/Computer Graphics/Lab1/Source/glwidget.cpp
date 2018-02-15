#include "glwidget.h"
#include <math.h>

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent){}

void GLWidget::initializeGL() {
    glClearColor(0, 0, 0, 1);
}

void GLWidget::paintGL() {
    double x0 = width / 2;
    double y0 = height  / 2;

    double radius = (height > width ? width : height) / 2;
    glClearColor(255, 255, 255, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glPointSize(5);
    glLineWidth(5);
    glBegin(primitives[primitiveType]);
        for (int i = 0; i < verticesCount; i++) {
            double alpha = 2 * M_PI * i / verticesCount + M_PI / 2;
            double x = x0 + radius*cos(alpha);
            double y = y0 + radius*sin(alpha);

            double beta = 2 * M_PI * i / verticesCount;
            glColor3d(sin(beta), sin(beta + 2*M_PI/3), sin(beta + 4*M_PI/3));

            glVertex2f(x, y);
        }
    glEnd();
    glFinish();
}

void GLWidget::resizeGL(int w, int h) {
    this->width = w;
     this->height = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);    /* ортографическая проекция */
    glLoadIdentity();
    glOrtho(0, w, 0, h, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget::setPrimitiveType(int primitiveType) {
    this->primitiveType = primitiveType;
    this->repaint();
}

void GLWidget::setVerticesCount(int verticesCount) {
    this->verticesCount = verticesCount;
    this->repaint();
}
