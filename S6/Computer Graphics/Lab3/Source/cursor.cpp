#include "cursor.h"
#include <math.h>

Cursor::Cursor() : Cursor(0, 1) {}

Cursor::Cursor(int circleFraction) {
    this->circleFraction = circleFraction;
}

Cursor::Cursor(int circleFraction, double moveDistance) {
    this->moveDistance = moveDistance;
    this->circleFraction = circleFraction;
}

void Cursor::setRotateAngle(int circleFraction) {
    // rotate angle is 2Pi/circleFraction
    this->circleFraction = circleFraction;
}

void Cursor::setMoveDistance(double moveDistance) {
    this->moveDistance = moveDistance;
}

void Cursor::setPosition(QPointF position) {
    this->position = position;
}

QPointF Cursor::getPosition() {
    return this->position;
}

double Cursor::getRotateAngle() {
    return 2*M_PI*(this->currentRotate) / (this->circleFraction);
}

void Cursor::rotate() {
    rotate(1);
}
void Cursor::rotate(int n) {
    // rotate n times in positive or negative (if n < 0) direction on the 2Pi/circleFraction rad
    this->currentRotate = (this->currentRotate + n) % (this->circleFraction);
}

QPointF Cursor::move() {
    return move(this->moveDistance);
}

QPointF Cursor::move(double distance) {
    double rotateAngle = this->getRotateAngle();
    this->position += distance*QPointF(cos(rotateAngle), sin(rotateAngle));
    return this->position;
}

