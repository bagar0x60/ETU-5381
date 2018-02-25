#include "glwidget.h"
#include <math.h>

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent){
    controlPoints = vector<QPointF>({QPointF(10, 10), QPointF(200, 300), QPointF(400, 300), QPointF(590, 10)});
    weights = vector<double>({1, 1, 1, 1});
    spline.setSplineDegree(2);
    spline.setControlPointsAndWeights(controlPoints, weights);
}

void GLWidget::initializeGL() {
    glClearColor(255, 255, 255, 1);
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);

    // draw all control points
    glColor3d(0, 0, 0);
    glPointSize(POINT_SIZE);
    glBegin(GL_POINTS);
    for (int i = 0; i < controlPoints.size(); i++) {
        QPointF pos = controlPoints[i];
        if (i == activePointIndex) {
            glColor3d(0, 0, 1);
            glVertex2d(pos.rx(), pos.ry());
            glColor3d(0, 0, 0);
        } else {
            glVertex2d(pos.rx(), pos.ry());
        }
    }
    glEnd();

    // draw control polygon
    glColor3d(0, 0, 0);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < controlPoints.size(); i++) {
        QPointF pos = controlPoints[i];
        glVertex2d(pos.rx(), pos.ry());
    }
    glEnd();

    // draw spline
    glColor3d(1, 0, 0);
    glLineWidth(2);
    glBegin(GL_LINE_STRIP);
        int pointsCount = 200;
        for (int i = 0; i < pointsCount; i++) {
            double t = (double)i / (double)pointsCount;
            QPointF pos = spline.compute(t);
            glVertex2d(pos.rx(), pos.ry());
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

// mouse
void GLWidget::mousePressEvent(QMouseEvent *event)
{
    movingPointIndex = -1;
    double x = event->pos().rx();
    double y = this->height - event->pos().ry();

    for (int i = 0; i < controlPoints.size(); i++) {
        QPointF p = controlPoints[i];
        if (abs(x - p.rx()) <= POINT_SIZE &&
            abs(y - p.ry()) <= POINT_SIZE)
        {
            if (event->buttons() == Qt::RightButton) {
                activePointIndex = i;
                emit weightChanged(weights[activePointIndex]);
            } else {
                movingPointIndex = i;
            }
            break;
        }
    }

    this->update();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (movingPointIndex != -1) {
        double x = event->pos().rx();
        double y = this->height - event->pos().ry();

        controlPoints[movingPointIndex] = QPointF(x, y);
        spline.setControlPoint(movingPointIndex, QPointF(x, y));

        this->update();
    }
}

// slots
void GLWidget::setControlPointsCount(int count) {
    int difference = count - controlPoints.size();

    if (difference < 0) {
        controlPoints.erase(controlPoints.end() + difference, controlPoints.end());
        weights.erase(weights.end() + difference, weights.end());
    } else {
        QPointF lastPoint = controlPoints[controlPoints.size() - 1];
        QPointF d = QPointF(4*POINT_SIZE, 0);
        while (difference--) {
            lastPoint += d;
            controlPoints.push_back(lastPoint);
            weights.push_back(1.);
        }
    }
    spline.setControlPointsAndWeights(controlPoints, weights);
    this->update();
}

void GLWidget::setSplineDegree(int degree) {
    spline.setSplineDegree(degree);
    this->update();
}

void GLWidget::setPointWeight(double weight) {
    weights[activePointIndex] = weight;
    spline.setWeight(activePointIndex, weight);
    this->update();
}
