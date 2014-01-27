#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "ui_mainwindow.h"
#include <vector>
#include <iostream>
#include <QTimer>
#include "graphicswidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT    
private:
    GraphicsWidget *gw;
    QTimer *timer;
    Ui::MainWindow *ui;
    std::vector<std::vector<bool>>* old_field;
    std::vector<std::vector<bool>>* new_field;

public:
    MainWindow(QWidget *parent = 0): QMainWindow(parent), ui(new Ui::MainWindow){
        ui->setupUi(this);
        gw = new GraphicsWidget;
        ui->verticalLayout->addWidget(gw);

        timer = new QTimer(this);
        timer->setInterval(100);
        connect(timer, SIGNAL(timeout()), this, SLOT(timerSlot()));

        connect(ui->startBtn, SIGNAL(clicked()), timer, SLOT(start()));
        connect(ui->finishBtn, SIGNAL(clicked()), timer, SLOT(stop()));

        old_field = createField(50, 50);
        new_field = createField(50, 50);

        gw->setField(old_field);
  }

    ~MainWindow(){
        delete ui;
        delete old_field;
        delete new_field;
    }

private:
    std::vector<std::vector<bool>>* createField(int col_count, int row_count){
        std::vector<std::vector<bool>>* m = new std::vector<std::vector<bool>>(col_count);
        for(int i = 0; i < col_count; ++i)
            (*m)[i] = std::vector<bool>(row_count);
        return m;
    }

    void calculateNext(){
        for(uint x = 1; x < old_field->size() - 1; ++x){
            for(uint y = 1; y < (*old_field->begin()).size() - 1; ++y){
                (*new_field)[x][y] = false;
                int nc = calcHeigb(x, y);
                if((!((*old_field)[x][y])) && nc == 3)
                    (*new_field)[x][y] = true;
                if(((*old_field)[x][y])){
                    if(nc == 3 || nc == 2)
                        (*new_field)[x][y] = true;
                    else
                        (*new_field)[x][y] = false;
                }
            }
        }
    }

    int calcHeigb(int x, int y){
        int res = 0;
        for(int i = x - 1; i <= x + 1; ++i)
            for(int j = y - 1; j <= y + 1; ++j)
                if((*old_field)[i][j])
                    ++res;
        if((*old_field)[x][y])
            --res;
        return res;
    }
    

private slots:
    void timerSlot(){
        calculateNext();
        gw->setField(new_field);

        std::swap(new_field, old_field);



    }
};

#endif
