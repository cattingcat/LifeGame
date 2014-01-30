#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "ui_mainwindow.h"
#include <vector>
#include <list>
#include <iostream>
#include <QTimer>
#include "graphicswidget.h"
#include "OpenGLGraphics.h"
#include <LifeEngine.h>
#include <ctime>

class MainWindow : public QMainWindow {
    Q_OBJECT    
private:
    QTimer *timer;
    Ui::MainWindow *ui;
    OpenGLGraphics *oglg;
    GraphicsWidget *gw;
    LifeEngine *le;

public:
    MainWindow(QWidget *parent = 0): QMainWindow(parent), ui(new Ui::MainWindow){
        ui->setupUi(this);
        gw = new GraphicsWidget();
        ui->verticalLayout->addWidget(gw);

        oglg = new OpenGLGraphics();
        ui->tab2Layout->addWidget(oglg);

        timer = new QTimer(this);
        timer->setInterval(100);

        le = new LifeEngine(100, 100);
        gw->setField(le->get_field());


        connect(timer, SIGNAL(timeout()), this, SLOT(nextTimer()));
        connect(ui->startBtn, SIGNAL(clicked()), timer, SLOT(start()));
        connect(ui->finishBtn, SIGNAL(clicked()), timer, SLOT(stop()));
        connect(ui->loadLogBtn, SIGNAL(clicked()), this, SLOT(loadLogSlot()));
    }

    ~MainWindow(){
        delete ui;
        delete le;
    }

private slots:
    void loadLogSlot(){
        std::cout<<"load log "<<std::endl;
        //oglg->drawLifeLog(gw->getLog());
    }
    void nextTimer(){
        clock_t c1 = clock();
        auto a = le->next();
        clock_t c2 = clock();
        gw->setField(a);
        clock_t c3 = clock();

        std::cout<< "next(): " << c2 - c1 << "  setField(): " << c3 - c2 <<std::endl;
    }



};

#endif
