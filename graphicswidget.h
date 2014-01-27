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
    std::vector<std::vector<bool>> field;
    std::list<std::vector<std::vector<bool>>> log;
    uint column_count;
    uint row_count;
    uint max_log_length;
    ThreadHelper *thr;

public:    
    GraphicsWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);
    ~GraphicsWidget();

private:
    int calcCellSize();
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent *e);
    std::vector<std::vector<bool>> createField();
    void calculateNext();
    int calculateNeighbor(int x, int y);

public slots:
    void setField(std::vector<std::vector<bool>> m);
    std::vector<std::vector<bool>> getField();
    void next();

    friend class ThreadHelper;
};

#endif
