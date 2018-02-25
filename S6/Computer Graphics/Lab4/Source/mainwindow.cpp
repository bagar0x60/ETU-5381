#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->degreeSpinBox, SIGNAL(valueChanged(int)),
            ui->widget, SLOT(setSplineDegree(int)));
    connect(ui->countSpinBox, SIGNAL(valueChanged(int)),
            ui->widget, SLOT(setControlPointsCount(int)));
    connect(ui->weightSpinBox, SIGNAL(valueChanged(double)),
            ui->widget, SLOT(setPointWeight(double)));
    connect(ui->widget, SIGNAL(weightChanged(double)),
            ui->weightSpinBox, SLOT(setValue(double)));

}

MainWindow::~MainWindow()
{
    delete ui;
}
