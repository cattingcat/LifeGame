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
#include <ogl_history.h>

class MainWindow : public QMainWindow {
    Q_OBJECT    
private:
    QTimer *timer;
    Ui::MainWindow *ui;
    OpenGLGraphics *oglg;
    HistoryWidget *hw;
    GraphicsWidget *gw;
    LifeEngine *le; 

public:
    MainWindow(QWidget *parent = 0): QMainWindow(parent), ui(new Ui::MainWindow){
        ui->setupUi(this);
        gw = new GraphicsWidget();
        ui->verticalLayout->addWidget(gw);

        oglg = new OpenGLGraphics();
        ui->tab2Layout->addWidget(oglg);

        hw = new HistoryWidget();
        ui->tab3Layout->addWidget(hw);

        timer = new QTimer(this);
        timer->setInterval(100);

        le = new LifeEngine(20, 20);
        gw->setField(le->get_field());
        oglg->setField(le->get_field());

        connect(timer, SIGNAL(timeout()), this, SLOT(nextTimer()));
        connect(ui->actionStart, SIGNAL(triggered()), timer, SLOT(start()));
        connect(ui->actionStop, SIGNAL(triggered()), timer, SLOT(stop()));
        connect(ui->actionRandomize, SIGNAL(triggered()), this, SLOT(randomize()));
    }

    ~MainWindow(){
        delete ui;
        delete le;
    }
private:
    void randomizeField(std::vector<std::vector<bool>>* field){
        int columns = field->size();
        int rows = field->begin()->size();
        for(int i = 0; i < columns; ++i){
            for(int j = 0; j < rows; ++j){
                (*field)[i][j] = (qrand() % 3 == 0);
            }
        }
    }

private slots:
    void nextTimer(){

        auto a = le->next();

        gw->setField(a);
        oglg->setField(a);
        hw->setLog(le->get_log());

    }

    void randomize(){
        auto field = le->get_field();
        randomizeField(field);
    }
};

#endif
