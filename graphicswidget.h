#ifndef GRAPHICSWIDGET_H
#define GRAPHICSWIDGET_H
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QWidget>
#include <vector>
#include <list>
#include <iostream>
#include <QPixmap>
#include <QThread>

class ThreadHelper;

class GraphicsWidget: public QWidget{
    Q_OBJECT
private:
    std::vector<std::vector<bool>>* field;

public:
    GraphicsWidget(): field(nullptr){

    }

protected:
    int calcCellSize(){
        int w = this->width();
        int h = this->height();
        int min_dimension = w > h ? h : w;
        min_dimension -= min_dimension % field->size();
        int cell_size = min_dimension / field->size();
        return cell_size;
    }

    virtual void paintEvent(QPaintEvent*) override {
        QPainter p(this);
        if(field != nullptr && !field->empty()){
            int cell_size = calcCellSize();
            int next_vert = 0;
            int next_hor = 0;

            int columns = field->size();
            int rows = field->begin()->size();

            for(int i = 0; i < columns; ++i, next_vert += cell_size){
                p.drawLine(next_vert, 0, next_vert, columns * cell_size);
                next_hor = 0;
                for(int j = 0; j < rows; ++j, next_hor += cell_size){
                    p.drawLine(0, next_hor, rows * cell_size, next_hor);
                    if((*field)[i][j])
                        p.fillRect(next_vert, next_hor, cell_size, cell_size, Qt::SolidPattern);
                }
            }
            p.drawLine(next_vert, 0, next_vert, columns * cell_size);
            p.drawLine(0, next_hor, rows * cell_size, next_hor);
        }
    }

    virtual void mousePressEvent(QMouseEvent *e) override {
        int cell_size = calcCellSize();
        int columns = field->size();
        int rows = field->begin()->size();

        if(e->x() < columns * cell_size && e->y() < rows * cell_size){
            int column = e->x() / cell_size;
            int row = e->y() / cell_size;
            bool b = (*field)[column][row];
            (*field)[column][row] = !b;
            repaint();
        }
    }

public:    
    void setField(std::vector<std::vector<bool>>* _field){
        field = _field;
        repaint();
    }
};

#endif
