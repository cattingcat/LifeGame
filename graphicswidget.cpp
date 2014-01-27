#include "graphicswidget.h"
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QWidget>
#include <vector>
#include <list>
#include <iostream>
#include <QPixmap>
#include <QThread>
#include "threadhelper.h"



GraphicsWidget::GraphicsWidget(QWidget* parent, Qt::WindowFlags f): QWidget(parent, f), thr(nullptr){
    column_count = 30;
    row_count = 30;
    max_log_length = 70;
    field = createField();
    setField(field);
}

GraphicsWidget::~GraphicsWidget(){
}

int GraphicsWidget::calcCellSize(){
    int w = this->width();
    int h = this->height();
    int min_dimension = w > h ? h : w;
    min_dimension -= min_dimension % column_count;
    int cell_size = min_dimension / column_count;
    return cell_size;
}

void GraphicsWidget::paintEvent(QPaintEvent*){
    QPainter p(this);
    if(!field.empty()){
        int cell_size = calcCellSize();
        int next_vert = 0;
        int next_hor = 0;
        for(int i = 0; i < column_count; ++i, next_vert += cell_size){
            p.drawLine(next_vert, 0, next_vert, column_count * cell_size);
            next_hor = 0;
            for(int j = 0; j < row_count; ++j, next_hor += cell_size){
                p.drawLine(0, next_hor, row_count * cell_size, next_hor);
                if(field[i][j])
                    p.fillRect(next_vert, next_hor, cell_size, cell_size, Qt::SolidPattern);
            }
        }
        p.drawLine(next_vert, 0, next_vert, column_count * cell_size);
        p.drawLine(0, next_hor, row_count * cell_size, next_hor);
    }
}

void GraphicsWidget::mousePressEvent(QMouseEvent *e){
    int cell_size = calcCellSize();
    if(e->x() < column_count * cell_size && e->y() < row_count * cell_size){
        int column = e->x() / cell_size;
        int row = e->y() / cell_size;
        bool b = field[column][row];
        field[column][row] = !b;
        repaint();
    }
}

std::vector<std::vector<bool>> GraphicsWidget::createField(){
    std::vector<std::vector<bool>> m = std::vector<std::vector<bool>>(column_count);
    for(uint i = 0; i < column_count; ++i)
        m[i] = std::vector<bool>(row_count);
    return m;
}

void GraphicsWidget::calculateNext(){
    std::vector<std::vector<bool>> last_field = *log.rbegin();
    for(uint x = 1; x < last_field.size() - 1; ++x){
        for(uint y = 1; y < (*last_field.begin()).size() - 1; ++y){
            field[x][y] = false;
            int nc = calculateNeighbor(x, y);
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

int GraphicsWidget::calculateNeighbor(int x, int y){
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

void GraphicsWidget::setField(std::vector<std::vector<bool>> m){
    field = m;
    repaint();
}

std::vector<std::vector<bool>> GraphicsWidget::getField(){
    return field;
}

void GraphicsWidget::next(){
    if(thr != nullptr){
        thr->wait();
        delete thr;
    }
    setField(field);
    thr = new ThreadHelper(this);
    thr->start();
}

