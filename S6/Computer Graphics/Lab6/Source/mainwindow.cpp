#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->isometricButton, SIGNAL(clicked()),
            ui->widget, SLOT(setIsometricProjection()));
    connect(ui->orthographicButton1, SIGNAL(clicked()),
            ui->widget, SLOT(setOrthographicProjection1()));
    connect(ui->orthographicButton2, SIGNAL(clicked()),
            ui->widget, SLOT(setOrthographicProjection2()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
