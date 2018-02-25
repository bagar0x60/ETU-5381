#include "nurbs.h"
#include <stdexcept>

NURBS::NURBS()
{
    this->firstKnot = 0;
    this->knotsDistance = 1.;

    this->controlPointsCount = 0;
    this->splineDegree = 0;
    this->knotsCount = controlPointsCount + splineDegree + 1;

    this->controlPoints = vector<QPointF>();
    this->weights = vector<double>();
}

void NURBS::setSplineDegree(int splineDegree) {
    this->splineDegree = splineDegree;
    this->knotsCount = controlPointsCount + splineDegree + 1;
}

void NURBS::setControlPointsAndWeights(vector<QPointF> controlPoints, vector<double> weights) {
    if (controlPoints.size() != weights.size())
        throw length_error("controlPoints size is not equile to weights size");
    this->controlPointsCount = controlPoints.size();
    this->knotsCount = controlPointsCount + splineDegree + 1;

    this->controlPoints = controlPoints;
    this->weights = weights;
}

void NURBS::setControlPoint(int controlPointIndex, QPointF controlPointValue) {
    this->controlPoints[controlPointIndex] = controlPointValue;
}

void NURBS::setWeight(int weightIndex, double weightValue) {
    this->weights[weightIndex] = weightValue;
}

QPointF NURBS::compute(double t) {
    //t = getKnot(splineDegree) + t*(getKnot(controlPointsCount) - getKnot(splineDegree));
    t *= knotsCount;
    QPointF result(0, 0);
    double weightSum = 0;

    for (int k = 0; k < controlPointsCount; k++) {
        double wN = weights[k]*N_recursive(splineDegree + 1, k, t);
        result += wN*controlPoints[k];
        weightSum += wN;
    }

    return result / weightSum;
}



double NURBS::getKnot(int k) {
    return this->firstKnot + k*(this->knotsDistance);
}

double NURBS::N_recursive(int q, int k, double t) {
    if (q == 1) {
        if (getKnot(k) <= t && t < getKnot(k + 1))
            return 1;
        else
            return 0;
    }
    else {
        double A = (t - getKnot(k)) / (getKnot(k + q - 1) - getKnot(k));
        double B = (getKnot(k + q) - t) / (getKnot(k + q) - getKnot(k + 1));

        return A*N_recursive(q - 1, k, t) + B*N_recursive(q - 1, k + 1, t);
    }
}
