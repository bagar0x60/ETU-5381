#include "glwidget.h"
#include <math.h>
#include <QVector3D>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent){
    prism = new RightPrism(10);
    prism->setRadius(prismSize);
    prism->setHeight(2*prismSize);

    rotate = QPointF(0, 0);
    mouseLastPosition  = QPointF(0, 0);
}

void GLWidget::initializeGL() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(255, 255, 255, 1);

}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(this->width / 2, this->height / 2, 0.0);
    glRotatef(rotate.ry(), 1, 0, 0);
    glRotatef(rotate.rx(), 0, 1, 0);
    glRotatef(360 / (2*prism->getEdgesCount()), 0, 0, 1);

    glLineWidth(2);

    prism->draw();

    glFinish();
}

void GLWidget::resizeGL(int w, int h) {
    this->width = w;
    this->height = h;
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);    // ортографическая проекция
    glLoadIdentity();
    glOrtho(0, w, 0, h, -500, 500);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


// mouse
void GLWidget::mousePressEvent(QMouseEvent *event)
{
    mouseLastPosition = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPointF currentPosition = event->pos();
    rotate += currentPosition - mouseLastPosition;
    mouseLastPosition = currentPosition;

    this->update();
}

void GLWidget::wheelEvent(QWheelEvent *event) {
    prismSize += event->pixelDelta().ry();
    prism->setRadius(prismSize);
    prism->setHeight(2*prismSize);

    this->update();
    event->accept();
}

void GLWidget::setIsometricProjection() {
    rotate = QPointF(45, 35.264);
    this->update();
}

void GLWidget::setOrthographicProjection1() {
    rotate = QPointF(0, 0);
    this->update();
}

void GLWidget::setOrthographicProjection2() {
    rotate = QPointF(0, 90);
    this->update();
}
