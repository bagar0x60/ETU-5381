#ifndef RIGHTPRISM_H
#define RIGHTPRISM_H

#include <vector>
#include <QVector3D>
#include <QGLWidget>
#include <QColor>

using namespace std;

class RightPrism
{
private:
    QColor figureColor, edgesColor;
    double r, h;
    int n;

    vector<QVector3D> upperPolyhedron;
    vector<QVector3D> downPolyhedron;

    void drawMeshEdges();
    void drawMeshPolygons();
    void computeVertices();
public:
    RightPrism();
    RightPrism(int n);

    void draw();
    void setFigureColor(QColor figureColor) ;
    void setEdgesColor(QColor edgesColor);

    void setEdgeSize(double size);
    void setHeight(double h);
    void setRadius(double r);

    int getEdgesCount();
};

#endif // RIGHTPRISM_H
