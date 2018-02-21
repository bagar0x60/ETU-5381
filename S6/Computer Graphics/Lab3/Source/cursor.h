#ifndef CURSOR_H
#define CURSOR_H

#include <QPointF>

class Cursor {
public:
    Cursor();
    Cursor(int circleFraction);
    Cursor(int circleFraction, double moveDistance);

    void setRotateAngle(int circleFraction);
    void resetRotate() {this->currentRotate = 0;}

    void setMoveDistance(double moveDistance);

    void setPosition(QPointF position);

    QPointF getPosition();
    double getRotateAngle();

    void rotate();
    void rotate(int n);

    QPointF move();
    QPointF move(double distance);

private:
    int circleFraction = 0;
    int currentRotate = 0;

    double moveDistance = 0;

    QPointF position;
};

#endif // CURSOR_H
