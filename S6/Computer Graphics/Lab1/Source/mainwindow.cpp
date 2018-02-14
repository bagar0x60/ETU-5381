#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), ui->widget, SLOT(setPrimitiveType(int)));
    connect(ui->horizontalScrollBar, SIGNAL(valueChanged(int)), ui->widget, SLOT(setVerticesCount(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}
