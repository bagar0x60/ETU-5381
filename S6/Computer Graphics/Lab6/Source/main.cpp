#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    //glutInit(&argc, argv);
    //glutInitDisplayMode(GLUT_RGB);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}