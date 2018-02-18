#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), ui->widget, SLOT(setPrimitiveType(int)));
    connect(ui->horizontalScrollBar, SIGNAL(valueChanged(int)), ui->widget, SLOT(setVerticesCount(int)));

    connect(ui->alphaTestFunc, SIGNAL(currentIndexChanged(int)), ui->widget, SLOT(setAlphaTestFunc(int)));
    connect(ui->alphaTestRef, SIGNAL(valueChanged(int)), ui->widget, SLOT(setAlphaTestRef(int)));

    connect(ui->sfactor, SIGNAL(currentIndexChanged(int)), ui->widget, SLOT(setBlendSfactor(int)));
    connect(ui->dfactor, SIGNAL(currentIndexChanged(int)), ui->widget, SLOT(setBlendDfactor(int)));

    connect(ui->isFlatCheckBox, SIGNAL(toggled(bool)), ui->widget, SLOT(setShadeModel(bool)));
}

MainWindow::~MainWindow()
{
    delete ui;
}
