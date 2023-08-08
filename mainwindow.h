#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void on_horizontalSlider_valueChanged(int value);
    ~MainWindow();

private slots:
    void on_actionOpen_vedio_File_triggered();

    void on_actClear_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
