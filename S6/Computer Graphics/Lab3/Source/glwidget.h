#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QMouseEvent>
#include "cursor.h"

class GLWidget : public QGLWidget
{
    Q_OBJECT

private:
    GLint width = 0, height = 0;
    int fractalDepth = 1;
    Cursor cursor;

    void drawFractal(int depth, double strokeLength);

public:
    GLWidget(QWidget *parent);

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

public slots:
    void setFractalDepth(int fractalDepth);
};

#endif // GLWIDGET_H
