#include "glwidget.h"
#include <math.h>

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent){
    this->cursor = Cursor(10);
}

void GLWidget::initializeGL() {
    glClearColor(255, 255, 255, 1);
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3d(0, 0, 0);
    cursor.setPosition(QPointF(width / 4, height / 4));
    cursor.resetRotate();

    glBegin(GL_LINE_STRIP);
        QPointF pos = cursor.getPosition();
        glVertex2d(pos.rx(), pos.ry());

        drawFractal(this->fractalDepth, width / 2);
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

void GLWidget::setFractalDepth(int fractalDepth) {
    this->fractalDepth = fractalDepth;
    this->update();
}


void GLWidget::drawFractal(int depth, double strokeLength) {
    if (depth == 1) {
        QPointF pos = cursor.move(strokeLength);
        glVertex2d(pos.rx(), pos.ry());
        return;
    }

    depth--;
    strokeLength *= (3. - sqrt(5)) / 2.;

    //R1 D R2 D R-4 D R-2 D R2 D R2 D R-1
    cursor.rotate(1);
    drawFractal(depth, strokeLength);
    cursor.rotate(2);
    drawFractal(depth, strokeLength);
    cursor.rotate(-4);
    drawFractal(depth, strokeLength);
    cursor.rotate(-2);
    drawFractal(depth, strokeLength);
    cursor.rotate(2);
    drawFractal(depth, strokeLength);
    cursor.rotate(2);
    drawFractal(depth, strokeLength);
    cursor.rotate(-1); //
}
