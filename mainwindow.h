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
    std::vector<std::vector<bool>> field;

    std::list<std::vector<std::vector<bool>>> log;

    uint column_count;
    uint row_count;

    uint max_log_length;

public:
    MainWindow(QWidget *parent = 0): QMainWindow(parent), ui(new Ui::MainWindow){
        column_count = 50;
        row_count = 50;
        max_log_length = 70;

        ui->setupUi(this);
        gw = new GraphicsWidget;
        ui->verticalLayout->addWidget(gw);

        timer = new QTimer(this);
        timer->setInterval(100);
        connect(timer, SIGNAL(timeout()), this, SLOT(timerSlot()));

        connect(ui->startBtn, SIGNAL(clicked()), timer, SLOT(start()));
        connect(ui->finishBtn, SIGNAL(clicked()), timer, SLOT(stop()));

        field = createField();

        gw->setField(field);
  }

    ~MainWindow(){
        delete ui;
    }

private:
    std::vector<std::vector<bool>> createField(){
        std::vector<std::vector<bool>> m = std::vector<std::vector<bool>>(column_count);
        for(int i = 0; i < column_count; ++i)
            m[i] = std::vector<bool>(row_count);
        return m;
    }

    void calculateNext(){
        std::vector<std::vector<bool>> last_field = *log.rbegin();
        for(uint x = 1; x < last_field.size() - 1; ++x){
            for(uint y = 1; y < (*last_field.begin()).size() - 1; ++y){
                field[x][y] = false;
                int nc = calcHeigb(x, y);
                if((!last_field[x][y]) && nc == 3)
                    field[x][y] = true;
                if((last_field[x][y])){
                    if(nc == 3 || nc == 2)
                        field[x][y] = true;
                    else
                        field[x][y] = false;
                }
            }
        }
    }

    int calcHeigb(int x, int y){
        std::vector<std::vector<bool>> last_field = *log.rbegin();
        int res = 0;
        for(int i = x - 1; i <= x + 1; ++i)
            for(int j = y - 1; j <= y + 1; ++j)
                if(last_field[i][j])
                    ++res;
        if(last_field[x][y])
            --res;
        return res;
    }

    void print_field(){
        for(uint x = 1; x < field.size() - 1; ++x){
            for(uint y = 1; y < (*field.begin()).size() - 1; ++y){
                std::cout<<field[x][y];
            }
            std::cout<<std::endl;
        }
        std::cout<<std::endl;
    }
    

private slots:
    void timerSlot(){
        if(log.size() < max_log_length){
            log.push_back(gw->getField());
        } else {
            log.pop_front();
            log.push_back(gw->getField());
        }
        field = createField();
        calculateNext();
        gw->setField(field);
    }
};

#endif
