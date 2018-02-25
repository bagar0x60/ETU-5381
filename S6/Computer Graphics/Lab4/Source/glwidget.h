#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QMouseEvent>
#include "nurbs.h"

class GLWidget : public QGLWidget
{
    Q_OBJECT

private:
    const int POINT_SIZE = 10;

    GLint width = 0, height = 0;

    NURBS spline;
    vector<QPointF> controlPoints;
    vector<double> weights;
    int movingPointIndex = 0;
    int activePointIndex = 1;

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

public:
    GLWidget(QWidget *parent);

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

public slots:
    void setControlPointsCount(int count);
    void setSplineDegree(int degree);
    void setPointWeight(double weight);

signals:
    void weightChanged(double wight);
};

#endif // GLWIDGET_H
