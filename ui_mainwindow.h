/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "csopenglwedget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actDrawRect;
    QAction *actClear;
    QAction *actionOpen_vedio_File;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_9;
    QVBoxLayout *verticalLayout_15;
    CSOpenGLWedget *openGLWidget;
    QHBoxLayout *horizontalLayout_7;
    QPushButton *pushButton_9;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *horizontalSpacer_5;
    QSpacerItem *horizontalSpacer_6;
    QPushButton *pushButton_12;
    QHBoxLayout *horizontalLayout_8;
    QPushButton *pushButton_10;
    QSpacerItem *horizontalSpacer_7;
    QPushButton *pushButton_11;
    QSpacerItem *horizontalSpacer_9;
    QVBoxLayout *verticalLayout_14;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_3;
    QSlider *horizontalSlider_18;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_7;
    QSpinBox *spinBox;
    QFrame *line;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_4;
    QPushButton *pushButton_2;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_2;
    QSlider *horizontalSlider;
    QVBoxLayout *verticalLayout_6;
    QLabel *label_5;
    QSlider *horizontalSlider_12;
    QFrame *line_2;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_7;
    QPushButton *pushButton_3;
    QVBoxLayout *verticalLayout_8;
    QLabel *label_4;
    QSlider *horizontalSlider_16;
    QVBoxLayout *verticalLayout_9;
    QLabel *label_6;
    QSlider *horizontalSlider_14;
    QFrame *line_3;
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *verticalLayout_10;
    QPushButton *pushButton_4;
    QVBoxLayout *verticalLayout_11;
    QLabel *label_8;
    QSlider *horizontalSlider_17;
    QSpacerItem *horizontalSpacer;
    QFrame *line_4;
    QHBoxLayout *horizontalLayout_5;
    QVBoxLayout *verticalLayout_12;
    QPushButton *pushButton_5;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *horizontalSpacer_3;
    QFrame *line_5;
    QHBoxLayout *horizontalLayout_6;
    QVBoxLayout *verticalLayout_13;
    QPushButton *pushButton_6;
    QPushButton *pushButton_7;
    QPushButton *pushButton_8;
    QMenuBar *menuBar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menu_3;
    QMenu *menu_4;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(621, 422);
        actDrawRect = new QAction(MainWindow);
        actDrawRect->setObjectName(QString::fromUtf8("actDrawRect"));
        actClear = new QAction(MainWindow);
        actClear->setObjectName(QString::fromUtf8("actClear"));
        actionOpen_vedio_File = new QAction(MainWindow);
        actionOpen_vedio_File->setObjectName(QString::fromUtf8("actionOpen_vedio_File"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        verticalLayout_15 = new QVBoxLayout();
        verticalLayout_15->setSpacing(6);
        verticalLayout_15->setObjectName(QString::fromUtf8("verticalLayout_15"));
        openGLWidget = new CSOpenGLWedget(centralWidget);
        openGLWidget->setObjectName(QString::fromUtf8("openGLWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(openGLWidget->sizePolicy().hasHeightForWidth());
        openGLWidget->setSizePolicy(sizePolicy);
        openGLWidget->setMinimumSize(QSize(100, 100));

        verticalLayout_15->addWidget(openGLWidget);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        pushButton_9 = new QPushButton(centralWidget);
        pushButton_9->setObjectName(QString::fromUtf8("pushButton_9"));
        pushButton_9->setCheckable(true);

        horizontalLayout_7->addWidget(pushButton_9);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_4);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_5);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_6);


        verticalLayout_15->addLayout(horizontalLayout_7);

        pushButton_12 = new QPushButton(centralWidget);
        pushButton_12->setObjectName(QString::fromUtf8("pushButton_12"));
        pushButton_12->setCheckable(true);

        verticalLayout_15->addWidget(pushButton_12);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        pushButton_10 = new QPushButton(centralWidget);
        pushButton_10->setObjectName(QString::fromUtf8("pushButton_10"));
        pushButton_10->setCheckable(true);

        horizontalLayout_8->addWidget(pushButton_10);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_7);

        pushButton_11 = new QPushButton(centralWidget);
        pushButton_11->setObjectName(QString::fromUtf8("pushButton_11"));

        horizontalLayout_8->addWidget(pushButton_11);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_9);


        verticalLayout_15->addLayout(horizontalLayout_8);

        verticalLayout_15->setStretch(0, 4);
        verticalLayout_15->setStretch(1, 1);
        verticalLayout_15->setStretch(3, 1);

        horizontalLayout_9->addLayout(verticalLayout_15);

        verticalLayout_14 = new QVBoxLayout();
        verticalLayout_14->setSpacing(6);
        verticalLayout_14->setObjectName(QString::fromUtf8("verticalLayout_14"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setCheckable(true);
        pushButton->setAutoRepeat(false);
        pushButton->setAutoExclusive(false);

        verticalLayout->addWidget(pushButton);


        horizontalLayout->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label_3);

        horizontalSlider_18 = new QSlider(centralWidget);
        horizontalSlider_18->setObjectName(QString::fromUtf8("horizontalSlider_18"));
        horizontalSlider_18->setOrientation(Qt::Horizontal);

        verticalLayout_2->addWidget(horizontalSlider_18);


        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        label_7 = new QLabel(centralWidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(label_7);

        spinBox = new QSpinBox(centralWidget);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));
        spinBox->setMinimum(1);
        spinBox->setMaximum(2);

        verticalLayout_3->addWidget(spinBox);


        horizontalLayout->addLayout(verticalLayout_3);


        verticalLayout_14->addLayout(horizontalLayout);

        line = new QFrame(centralWidget);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout_14->addWidget(line);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setCheckable(true);

        verticalLayout_4->addWidget(pushButton_2);


        horizontalLayout_2->addLayout(verticalLayout_4);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(label_2);

        horizontalSlider = new QSlider(centralWidget);
        horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
        horizontalSlider->setOrientation(Qt::Horizontal);

        verticalLayout_5->addWidget(horizontalSlider);


        horizontalLayout_2->addLayout(verticalLayout_5);

        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setAlignment(Qt::AlignCenter);

        verticalLayout_6->addWidget(label_5);

        horizontalSlider_12 = new QSlider(centralWidget);
        horizontalSlider_12->setObjectName(QString::fromUtf8("horizontalSlider_12"));
        horizontalSlider_12->setOrientation(Qt::Horizontal);

        verticalLayout_6->addWidget(horizontalSlider_12);


        horizontalLayout_2->addLayout(verticalLayout_6);


        verticalLayout_14->addLayout(horizontalLayout_2);

        line_2 = new QFrame(centralWidget);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        verticalLayout_14->addWidget(line_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        pushButton_3 = new QPushButton(centralWidget);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setCheckable(true);

        verticalLayout_7->addWidget(pushButton_3);


        horizontalLayout_3->addLayout(verticalLayout_7);

        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setAlignment(Qt::AlignCenter);

        verticalLayout_8->addWidget(label_4);

        horizontalSlider_16 = new QSlider(centralWidget);
        horizontalSlider_16->setObjectName(QString::fromUtf8("horizontalSlider_16"));
        horizontalSlider_16->setOrientation(Qt::Horizontal);

        verticalLayout_8->addWidget(horizontalSlider_16);


        horizontalLayout_3->addLayout(verticalLayout_8);

        verticalLayout_9 = new QVBoxLayout();
        verticalLayout_9->setSpacing(6);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setAlignment(Qt::AlignCenter);

        verticalLayout_9->addWidget(label_6);

        horizontalSlider_14 = new QSlider(centralWidget);
        horizontalSlider_14->setObjectName(QString::fromUtf8("horizontalSlider_14"));
        horizontalSlider_14->setMaximum(100);
        horizontalSlider_14->setOrientation(Qt::Horizontal);

        verticalLayout_9->addWidget(horizontalSlider_14);


        horizontalLayout_3->addLayout(verticalLayout_9);


        verticalLayout_14->addLayout(horizontalLayout_3);

        line_3 = new QFrame(centralWidget);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        verticalLayout_14->addWidget(line_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        verticalLayout_10 = new QVBoxLayout();
        verticalLayout_10->setSpacing(6);
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        pushButton_4 = new QPushButton(centralWidget);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setCheckable(true);

        verticalLayout_10->addWidget(pushButton_4);


        horizontalLayout_4->addLayout(verticalLayout_10);

        verticalLayout_11 = new QVBoxLayout();
        verticalLayout_11->setSpacing(6);
        verticalLayout_11->setObjectName(QString::fromUtf8("verticalLayout_11"));
        label_8 = new QLabel(centralWidget);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setAlignment(Qt::AlignCenter);

        verticalLayout_11->addWidget(label_8);

        horizontalSlider_17 = new QSlider(centralWidget);
        horizontalSlider_17->setObjectName(QString::fromUtf8("horizontalSlider_17"));
        horizontalSlider_17->setOrientation(Qt::Horizontal);

        verticalLayout_11->addWidget(horizontalSlider_17);


        horizontalLayout_4->addLayout(verticalLayout_11);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);


        verticalLayout_14->addLayout(horizontalLayout_4);

        line_4 = new QFrame(centralWidget);
        line_4->setObjectName(QString::fromUtf8("line_4"));
        line_4->setFrameShape(QFrame::HLine);
        line_4->setFrameShadow(QFrame::Sunken);

        verticalLayout_14->addWidget(line_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        verticalLayout_12 = new QVBoxLayout();
        verticalLayout_12->setSpacing(6);
        verticalLayout_12->setObjectName(QString::fromUtf8("verticalLayout_12"));
        pushButton_5 = new QPushButton(centralWidget);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));
        pushButton_5->setCheckable(true);

        verticalLayout_12->addWidget(pushButton_5);


        horizontalLayout_5->addLayout(verticalLayout_12);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_2);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_3);


        verticalLayout_14->addLayout(horizontalLayout_5);

        line_5 = new QFrame(centralWidget);
        line_5->setObjectName(QString::fromUtf8("line_5"));
        line_5->setFrameShape(QFrame::HLine);
        line_5->setFrameShadow(QFrame::Sunken);

        verticalLayout_14->addWidget(line_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        verticalLayout_13 = new QVBoxLayout();
        verticalLayout_13->setSpacing(6);
        verticalLayout_13->setObjectName(QString::fromUtf8("verticalLayout_13"));
        pushButton_6 = new QPushButton(centralWidget);
        pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));
        pushButton_6->setCheckable(true);

        verticalLayout_13->addWidget(pushButton_6);


        horizontalLayout_6->addLayout(verticalLayout_13);

        pushButton_7 = new QPushButton(centralWidget);
        pushButton_7->setObjectName(QString::fromUtf8("pushButton_7"));

        horizontalLayout_6->addWidget(pushButton_7);

        pushButton_8 = new QPushButton(centralWidget);
        pushButton_8->setObjectName(QString::fromUtf8("pushButton_8"));

        horizontalLayout_6->addWidget(pushButton_8);


        verticalLayout_14->addLayout(horizontalLayout_6);


        horizontalLayout_9->addLayout(verticalLayout_14);

        horizontalLayout_9->setStretch(0, 4);
        horizontalLayout_9->setStretch(1, 1);

        gridLayout->addLayout(horizontalLayout_9, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 621, 28));
        menu = new QMenu(menuBar);
        menu->setObjectName(QString::fromUtf8("menu"));
        menu_2 = new QMenu(menuBar);
        menu_2->setObjectName(QString::fromUtf8("menu_2"));
        menu_3 = new QMenu(menuBar);
        menu_3->setObjectName(QString::fromUtf8("menu_3"));
        menu_4 = new QMenu(menuBar);
        menu_4->setObjectName(QString::fromUtf8("menu_4"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menu_2->menuAction());
        menuBar->addAction(menu_3->menuAction());
        menuBar->addAction(menu_4->menuAction());
        menu->addAction(actionOpen_vedio_File);
        mainToolBar->addAction(actDrawRect);
        mainToolBar->addAction(actClear);

        retranslateUi(MainWindow);
        QObject::connect(spinBox, SIGNAL(valueChanged(int)), openGLWidget, SLOT(on_sliderValue_waveType_clicked(int)));
        QObject::connect(horizontalSlider, SIGNAL(valueChanged(int)), openGLWidget, SLOT(on_sliderValue_focussensitivity_clicked(int)));
        QObject::connect(horizontalSlider_12, SIGNAL(valueChanged(int)), openGLWidget, SLOT(on_sliderValue_focushighThreshold_clicked(int)));
        QObject::connect(horizontalSlider_16, SIGNAL(valueChanged(int)), openGLWidget, SLOT(on_sliderValue_zebralowThreshold_clicked(int)));
        QObject::connect(horizontalSlider_14, SIGNAL(valueChanged(int)), openGLWidget, SLOT(on_sliderValue_zebrahigThreshold_clicked(int)));
        QObject::connect(horizontalSlider_17, SIGNAL(valueChanged(int)), openGLWidget, SLOT(on_sliderValue_sharpensharpenStr_clicked(int)));
        QObject::connect(pushButton_7, SIGNAL(clicked()), openGLWidget, SLOT(on_pushButton_lutFile1_clicked()));
        QObject::connect(pushButton_8, SIGNAL(clicked()), openGLWidget, SLOT(on_pushButton_lutFile2_clicked()));
        QObject::connect(pushButton, SIGNAL(clicked(bool)), openGLWidget, SLOT(on_pushButton_wave_clicked(bool)));
        QObject::connect(pushButton_2, SIGNAL(clicked(bool)), openGLWidget, SLOT(on_pushButton_focus_clicked(bool)));
        QObject::connect(pushButton_3, SIGNAL(clicked(bool)), openGLWidget, SLOT(on_pushButton_zebra_clicked(bool)));
        QObject::connect(pushButton_4, SIGNAL(clicked(bool)), openGLWidget, SLOT(on_pushButton_sharpen_clicked(bool)));
        QObject::connect(pushButton_5, SIGNAL(clicked(bool)), openGLWidget, SLOT(on_pushButton_false_clicked(bool)));
        QObject::connect(pushButton_6, SIGNAL(clicked(bool)), openGLWidget, SLOT(on_pushButton_lut_clicked(bool)));
        QObject::connect(pushButton_9, SIGNAL(clicked(bool)), openGLWidget, SLOT(on_pushButton_hist_clicked(bool)));
        QObject::connect(pushButton_10, SIGNAL(clicked(bool)), openGLWidget, SLOT(on_pushButton_vector_clicked(bool)));
        QObject::connect(pushButton_11, SIGNAL(clicked()), openGLWidget, SLOT(on_pushButton_savepicture_clicked()));
        QObject::connect(horizontalSlider_18, SIGNAL(valueChanged(int)), openGLWidget, SLOT(on_sliderValue_waveAlpha_clicked(int)));
        QObject::connect(pushButton_12, SIGNAL(clicked(bool)), openGLWidget, SLOT(on_pushButton_mag_clicked(bool)));
        QObject::connect(pushButton_12, SIGNAL(clicked(bool)), openGLWidget, SLOT(on_pushButton_mag_clicked(bool)));
        QObject::connect(pushButton_12, SIGNAL(clicked(bool)), openGLWidget, SLOT(on_pushButton_magn_clicked(bool)));
        QObject::connect(pushButton_12, SIGNAL(clicked(bool)), openGLWidget, SLOT(on_pushButton_magn_clicked(bool)));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        actDrawRect->setText(QApplication::translate("MainWindow", "\347\273\230\345\210\266\347\237\251\345\275\242", nullptr));
        actClear->setText(QApplication::translate("MainWindow", "\346\270\205\347\251\272\346\230\276\347\244\272", nullptr));
        actionOpen_vedio_File->setText(QApplication::translate("MainWindow", "Open vedio File", nullptr));
        pushButton_9->setText(QApplication::translate("MainWindow", "Hist", nullptr));
        pushButton_12->setText(QApplication::translate("MainWindow", "Mag", nullptr));
        pushButton_10->setText(QApplication::translate("MainWindow", "VectorScope", nullptr));
        pushButton_11->setText(QApplication::translate("MainWindow", "save picture", nullptr));
        pushButton->setText(QApplication::translate("MainWindow", "WaveForm", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "Alpha", nullptr));
        label_7->setText(QApplication::translate("MainWindow", "Type", nullptr));
        pushButton_2->setText(QApplication::translate("MainWindow", "Focus", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "sensitivity", nullptr));
        label_5->setText(QApplication::translate("MainWindow", "Peaking", nullptr));
        pushButton_3->setText(QApplication::translate("MainWindow", "Zebra", nullptr));
        label_4->setText(QApplication::translate("MainWindow", "LowThread", nullptr));
        label_6->setText(QApplication::translate("MainWindow", "HigThread", nullptr));
        pushButton_4->setText(QApplication::translate("MainWindow", "Sharpen", nullptr));
        label_8->setText(QApplication::translate("MainWindow", "sharpness", nullptr));
        pushButton_5->setText(QApplication::translate("MainWindow", "FalseColor", nullptr));
        pushButton_6->setText(QApplication::translate("MainWindow", "3D LUT", nullptr));
        pushButton_7->setText(QApplication::translate("MainWindow", "LUT1", nullptr));
        pushButton_8->setText(QApplication::translate("MainWindow", "LUT2", nullptr));
        menu->setTitle(QApplication::translate("MainWindow", "\346\226\207\344\273\266", nullptr));
        menu_2->setTitle(QApplication::translate("MainWindow", "\346\237\245\347\234\213", nullptr));
        menu_3->setTitle(QApplication::translate("MainWindow", "\350\256\276\347\275\256", nullptr));
        menu_4->setTitle(QApplication::translate("MainWindow", "\345\270\256\345\212\251", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
