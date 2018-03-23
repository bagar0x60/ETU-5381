#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QMouseEvent>
#include "rightprism.h"

class GLWidget : public QGLWidget
{
    Q_OBJECT

private:
    GLint width = 0, height = 0;
    RightPrism *prism;
    QPointF mouseLastPosition;
    QPointF rotate;
    double prismSize = 100;

public:
    GLWidget(QWidget *parent);

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

public slots:
    void setIsometricProjection();
    void setOrthographicProjection1();
    void setOrthographicProjection2();
};

#endif // GLWIDGET_H
