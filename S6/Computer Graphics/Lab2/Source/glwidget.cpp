#include "glwidget.h"
#include <math.h>

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent){}

void GLWidget::initializeGL() {
    glClearColor(255, 255, 255, 1);
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
}

void GLWidget::paintGL() {
    glDisable(GL_SCISSOR_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    if (isScissorTestEnabled) glEnable(GL_SCISSOR_TEST);

    drawFigures();
    drawFrame();
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
    this->update();
}

void GLWidget::setVerticesCount(int verticesCount) {
    this->verticesCount = verticesCount;
    this->update();
}

// Alpha Test  --------------------------
void GLWidget::setAlphaTestFunc(int alphaTestFuncIndex) {
    this->alphaTestFun = this->alphaTestFuns[alphaTestFuncIndex];
    glAlphaFunc(alphaTestFun, alphaTestRef);
    this->update();
}

void GLWidget::setAlphaTestRef(int alphaTestRefPercent) {
    this->alphaTestRef = (float)alphaTestRefPercent / 100.;
    glAlphaFunc(alphaTestFun, alphaTestRef);
    this->update();
}
//---------------------------------------

// Blend    -----------------------------
void GLWidget::setBlendSfactor(int blendSfactorIndex) {
    blendSfactor = blendSfactors[blendSfactorIndex];
    glBlendFunc(blendSfactor, blendDfactor);
    update();
}

void GLWidget::setBlendDfactor(int blendDfactorIndex) {
    blendDfactor = blendDfactors[blendDfactorIndex];
    glBlendFunc(blendSfactor, blendDfactor);
    update();
}
//---------------------------------------

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    mousePressPosition = mouseCurrentPosition = event->pos();
    isMousePressed = true;
    isScissorTestEnabled = false;
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    mouseCurrentPosition = event->pos();
    this->update();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    isMousePressed = false;
    if (mouseCurrentPosition != mousePressPosition) {
        this->setupScissorsTest();
        isScissorTestEnabled = true;
    }
    this->update();
}

void GLWidget::setupScissorsTest() {
    int x = qMin(mousePressPosition.rx(), mouseCurrentPosition.rx());
    int w = qMax(mousePressPosition.rx(), mouseCurrentPosition.rx()) - x;
    int y = height - qMax(mousePressPosition.ry(), mouseCurrentPosition.ry());
    int h = height - qMin(mousePressPosition.ry(), mouseCurrentPosition.ry()) - y;

    glScissor(x, y, w, h);
}

void GLWidget::drawFigures() {
    double x0 = width / 2;
    double y0 = height  / 2;

    double radius = (height > width ? width : height) / 2;

    glPointSize(5);
    glLineWidth(5);
    glBegin(primitives[primitiveType]);
        for (int i = 0; i < verticesCount; i++) {
            double alpha = 2 * M_PI * i / verticesCount + M_PI / 2;
            double x = x0 + radius*cos(alpha);
            double y = y0 + radius*sin(alpha);

            double beta = 2 * M_PI * i / verticesCount;
            glColor4d(sin(beta), sin(beta + 2*M_PI/3), sin(beta + 4*M_PI/3), (double)i / (double)verticesCount);

            glVertex2f(x, y);
        }
    glEnd();

    glFinish();
}

void GLWidget::drawFrame() {
    if (isMousePressed) {
        glLineWidth(2);
        glColor3d(0, 0, 0);
        int x0 = mousePressPosition.rx(), y0 = height - mousePressPosition.ry();
        int x1 = mouseCurrentPosition.rx(), y1 = height - mouseCurrentPosition.ry();
        glBegin(GL_LINE_LOOP);
            glVertex3i(x0, y0, 1);
            glVertex3i(x0, y1, 1);
            glVertex3i(x1, y1, 1);
            glVertex3i(x1, y0, 1);
        glEnd();
    }
}

void GLWidget::setShadeModel(bool isFlat) {
    if (isFlat) {
        glShadeModel(GL_FLAT);
    } else {
        glShadeModel(GL_SMOOTH);
    }
    update();
}
