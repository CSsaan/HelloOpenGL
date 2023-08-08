#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "csopenglwedget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
//    setWindowOpacity(0.5); // 透明度
    ui->setupUi(this);
//    setCentralWidget(ui->openGLWidget); //  铺满全屏
//    setWindowFlags(Qt::WindowStaysOnTopHint);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actClear_triggered()
{
    qDebug() << "YOU cannot clear!";
}

void MainWindow::on_actionOpen_vedio_File_triggered()
{
    qDebug() << "YOU cannot open!";
}
