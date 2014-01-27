#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "ui_mainwindow.h"
#include <vector>
#include <list>
#include <iostream>
#include <QTimer>
#include "graphicswidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT    
private:
    GraphicsWidget *gw;
    QTimer *timer;
    Ui::MainWindow *ui;

public:
    MainWindow(QWidget *parent = 0): QMainWindow(parent), ui(new Ui::MainWindow){
        ui->setupUi(this);
        gw = new GraphicsWidget;
        ui->verticalLayout->addWidget(gw);
        timer = new QTimer(this);
        timer->setInterval(100);
        connect(timer, SIGNAL(timeout()), gw, SLOT(next()));
        connect(ui->startBtn, SIGNAL(clicked()), timer, SLOT(start()));
        connect(ui->finishBtn, SIGNAL(clicked()), timer, SLOT(stop()));
    }

    ~MainWindow(){
        delete ui;
    }
};

#endif
