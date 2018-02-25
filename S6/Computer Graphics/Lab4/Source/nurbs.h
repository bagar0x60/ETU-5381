#ifndef NURBS_H
#define NURBS_H

#include <QPointF>
#include <vector>

using namespace std;

class NURBS
{
private:
    int controlPointsCount = 0;
    int splineDegree = 0;
    int knotsCount = 1;
    double knotsDistance = 0;
    double firstKnot = 0;

    vector<QPointF> controlPoints;
    vector<double> weights;

    double getKnot(int k);
    double N_recursive(int q, int k, double t);
public:
    NURBS();

    void setSplineDegree(int splineDegree);

    void setControlPointsAndWeights(vector<QPointF> controlPoints, vector<double> weights);
    void setControlPoint(int controlPointIndex, QPointF controlPointValue);
    void setWeight(int weightIndex, double weightValue);

    QPointF compute(double t);
};

#endif // NURBS_H
