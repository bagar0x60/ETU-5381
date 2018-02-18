#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QMouseEvent>



class GLWidget : public QGLWidget
{
    Q_OBJECT

private:
    GLint width = 0, height = 0;
    int primitiveType = 0;
    int const primitives[10] = {GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP, GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_QUADS, GL_QUAD_STRIP, GL_POLYGON};
    int verticesCount = 3;

    int const alphaTestFuns[8] = {GL_NEVER, GL_LESS, GL_EQUAL, GL_LEQUAL, GL_GREATER, GL_NOTEQUAL, GL_GEQUAL, GL_ALWAYS};
    int alphaTestFun = GL_ALWAYS;
    float alphaTestRef = 0;

    int const blendSfactors[15] = {GL_ZERO, GL_ONE, GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR, GL_DST_COLOR,
                                  GL_ONE_MINUS_DST_COLOR, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA,
                                  GL_ONE_MINUS_DST_ALPHA, GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_COLOR,
                                  GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA, GL_SRC_ALPHA_SATURATE};
    int const blendDfactors[14] = {GL_ZERO, GL_ONE, GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR, GL_DST_COLOR,
                                   GL_ONE_MINUS_DST_COLOR, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA,
                                   GL_ONE_MINUS_DST_ALPHA, GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_COLOR,
                                   GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA};
    int blendSfactor = GL_ONE;
    int blendDfactor = GL_ZERO;

    QPoint mousePressPosition, mouseCurrentPosition;

    bool isScissorTestEnabled = false;
    bool isMousePressed = false;

    void setupScissorsTest();
    void drawFigures();
    void drawFrame();
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

public:
    GLWidget(QWidget *parent);

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

public slots:
    void setPrimitiveType(int primitiveType);
    void setVerticesCount(int verticesCount);

    void setAlphaTestFunc(int alphaTestFuncIndex);
    void setAlphaTestRef(int alphaTestRefPercent);

    void setBlendSfactor(int blendSfactorIndex);
    void setBlendDfactor(int blendDfactorIndex);

    void setShadeModel(bool isFlat);
};

#endif // GLWIDGET_H
