#ifndef GRAPHICSWIDGET_H
#define GRAPHICSWIDGET_H
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QWidget>
#include <vector>
#include <iostream>
#include <QPixmap>


class GraphicsWidget: public QWidget{
    Q_OBJECT
private:
    std::vector<std::vector<bool>> *_field;

public:    
    GraphicsWidget(QWidget* parent = 0, Qt::WindowFlags f = 0): QWidget(parent, f), _field(nullptr){
    }

    ~GraphicsWidget(){
    }

private:
    int calcCellSize(int col_count, int row_count){
        int w = this->width();
        int h = this->height();
        int min_dimension = w > h ? h : w;
        min_dimension -= min_dimension % col_count;
        int cell_size = min_dimension / col_count;
        return cell_size;
    }

    void paintEvent(QPaintEvent*){
        QPainter p(this);

        if(_field != nullptr){
            int col_count = _field->size();
            int row_count = _field->begin()->size();

            int cell_size = calcCellSize(col_count, row_count);

            int next_vert = 0;
            int next_hor = 0;
            for(int i = 0; i < col_count; ++i, next_vert += cell_size){
                p.drawLine(next_vert, 0, next_vert, col_count * cell_size);
                next_hor = 0;
                for(int j = 0; j < row_count; ++j, next_hor += cell_size){
                    p.drawLine(0, next_hor, row_count * cell_size, next_hor);
                    if((*_field)[i][j])
                        p.fillRect(next_vert, next_hor, cell_size, cell_size, Qt::SolidPattern);
                }
            }
            p.drawLine(next_vert, 0, next_vert, col_count * cell_size);
            p.drawLine(0, next_hor, row_count * cell_size, next_hor);
        }
    }

    void mousePressEvent(QMouseEvent *e){
        int col_count = _field->size();
        int row_count = _field->begin()->size();
        int cell_size = calcCellSize(col_count, row_count);
        if(e->x() < col_count * cell_size && e->y() < row_count * cell_size){
            int column = e->x() / cell_size;
            int row = e->y() / cell_size;
            bool b = (*_field)[column][row];
            (*_field)[column][row] = !b;
            repaint();
        }
    }

public:
    void setField(std::vector<std::vector<bool>>* m){
        _field = m;
        repaint();
    }

    std::vector<std::vector<bool>>* getField(){
        return _field;
    }
};

#endif
