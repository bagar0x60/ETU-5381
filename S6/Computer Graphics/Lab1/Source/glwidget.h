#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>

class GLWidget : public QGLWidget
{
    Q_OBJECT

private:
    GLint width = 0, height = 0;
    int primitiveType = 0;
    int const primitives[10] = {GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP, GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_QUADS, GL_QUAD_STRIP, GL_POLYGON};
    int verticesCount = 3;

public:
    GLWidget(QWidget *parent);

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

public slots:
    void setPrimitiveType(int primitiveType);
    void setVerticesCount(int verticesCount);
};

#endif // GLWIDGET_H
