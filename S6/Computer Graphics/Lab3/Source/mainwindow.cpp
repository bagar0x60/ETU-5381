#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->depthScrollBar, SIGNAL(valueChanged(int)), ui->widget, SLOT(setFractalDepth(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}
