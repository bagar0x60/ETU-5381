#include "rightprism.h"
#include <math.h>

RightPrism::RightPrism() : RightPrism(10) {}

RightPrism::RightPrism(int n) {
    this->n = n;
    setHeight(100);
    setEdgeSize(100);

    figureColor = QColor(255, 255, 255);
    edgesColor = QColor(0, 0, 0, 127);

    computeVertices();
}

void RightPrism::draw() {
    glDisable(GL_POLYGON_OFFSET_FILL);
    glColor3ub(edgesColor.red(), edgesColor.green(), edgesColor.blue());
    drawMeshEdges();
    glEnable(GL_POLYGON_OFFSET_FILL);

    glPolygonOffset(1.0, 1.0);
    glColor3ub(figureColor.red(), figureColor.green(), figureColor.blue());
    drawMeshPolygons();
}

void RightPrism::drawMeshEdges() {
    QVector3D p;
    glBegin(GL_LINE_LOOP);
        for (int i = 0; i < n; i++) {
            p = upperPolyhedron[i];
            glVertex3f(p.x(), p.y(), p.z());
        }
    glEnd();

    glBegin(GL_LINE_LOOP);
        for (int i = 0; i < n; i++) {
            p = downPolyhedron[i];
            glVertex3f(p.x(), p.y(), p.z());
        }
    glEnd();

    glBegin(GL_LINES);
        for (int i = 0; i < n; i++) {
            p = upperPolyhedron[i];
            glVertex3f(p.x(), p.y(), p.z());
            p = downPolyhedron[i];
            glVertex3f(p.x(), p.y(), p.z());
        }
    glEnd();
}

void RightPrism::drawMeshPolygons() {
    QVector3D p;
    // upper polygon
    glBegin(GL_POLYGON);
        for (int i = 0; i < n; i++) {
            p = upperPolyhedron[i];
            glVertex3f(p.x(), p.y(), p.z());
        }
    glEnd();

    // down polygon
    glBegin(GL_POLYGON);
        for (int i = 0; i < n; i++) {
            p = downPolyhedron[i];
            glVertex3f(p.x(), p.y(), p.z());
        }
    glEnd();

    // edges

    for (int i = 0; i < n; i++) {
        glBegin(GL_POLYGON);
            p = upperPolyhedron[i];
            glVertex3f(p.x(), p.y(), p.z());
            p = downPolyhedron[i];
            glVertex3f(p.x(), p.y(), p.z());
            p = downPolyhedron[(i + 1) % n];
            glVertex3f(p.x(), p.y(), p.z());
            p = upperPolyhedron[(i + 1) % n];
            glVertex3f(p.x(), p.y(), p.z());
        glEnd();
    }

}

void RightPrism::computeVertices() {
    upperPolyhedron.clear();
    downPolyhedron.clear();

    for (int i = 0; i < n; i++) {
        double alpha =  2 * M_PI * (double)i / (double)n;

        upperPolyhedron.push_back(QVector3D(r*cos(alpha), r*sin(alpha), h / 2));
        downPolyhedron.push_back(QVector3D(r*cos(alpha), r*sin(alpha), -h / 2));
    }
}

void RightPrism::setFigureColor(QColor figureColor) {
    this->figureColor = figureColor;
}

void RightPrism::setEdgesColor(QColor edgesColor) {
    this->edgesColor = edgesColor;
}

void RightPrism::setEdgeSize(double size) {
    double alpha = 2*M_PI / n;
    this->r = size / sqrt(2*(1 - cos(alpha)));
    computeVertices();
}

void RightPrism::setHeight(double h) {
    this->h = h;
    computeVertices();
}

void RightPrism::setRadius(double r) {
    this->r = r;
    computeVertices();
}

int RightPrism::getEdgesCount() {
    return n;
}




