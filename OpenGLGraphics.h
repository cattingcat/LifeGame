#ifndef OPENGLGRAPHICS_H
#define OPENGLGRAPHICS_H
#include <QGLWidget>
#include <QWheelEvent>
#define M_PI 3.14159265358979323846
#include <iostream>

class OpenGLGraphics: public QGLWidget {
    Q_OBJECT
private:
    int w;
    int h;
    std::vector<std::vector<bool>>* _field;

public:
    OpenGLGraphics(QWidget* parent = 0): QGLWidget(parent), _field(nullptr){
        w = 1000;
        h = 1000;
    }

protected:
    //QOpenGLWidget
    virtual void initializeGL() override{
        glClearColor(0, 0, 0, 0);
    }

    virtual void resizeGL(int width, int height) override{
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glViewport(0, 0, width, height);
        float tmp = (float)width / height;
        if(width > height){
            glOrtho(0, w * tmp, h, 0, -1, 1);
            //w = (float)w * tmp;
        } else {
            glOrtho(0, w, h * tmp, 0, -1, 1);
            //h = (float)h * tmp;
        }
    }

    virtual void paintGL() override{
        glClear(GL_COLOR_BUFFER_BIT);
        if(_field != nullptr && !_field->empty()){
            float cell_size = getCellSize();
            int n = _field->size();
            for(int i = 0; i < n; ++i){
                drawLine(0, cell_size * i, cell_size * n, cell_size * i);
                drawLine(cell_size * i, 0, cell_size * i, cell_size * n);
                for(int j = 0; j < n; ++j){
                    int x = cell_size * i;
                    int y = cell_size * j;
                    if((*_field)[i][j])
                        drawSquare(x + 1, y + 1, 0, cell_size - 1);
                }
            }
            drawLine(0, cell_size * n, cell_size * n, cell_size * n);
            drawLine(cell_size * n, 0, cell_size * n, cell_size * n);
        }
    }

    // QWidget events
    virtual void mousePressEvent(QMouseEvent *e) override{
        float cell_size = getCellSize();
        int x = e->x();
        int y = e->y();
        // translate to OGL coords
        x = (float)(x * w) / width();
        y = (float)(y * h) / height();
        int column = x / cell_size;
        int row = y / cell_size;
        bool b = (*_field)[column][row];
        (*_field)[column][row] = !b;
        repaint();
    }

private:
    void drawSquare(int x, int y, int z, int size){
        glBegin(GL_QUADS);
        glColor3f(1, 0, 0);
        glVertex3i(x, y, z);
        glColor3f(0, 1, 0);
        glVertex3i(x + size, y, z);
        glColor3f(0, 0, 1);
        glVertex3i(x + size, y + size, z);
        glColor3f(1, 1, 1);
        glVertex3i(x, y + size,z );
        glEnd();
    }

    void drawLine(int x1, int y1, int x2, int y2, int z1 = 0, int z2 = 0){
        glBegin(GL_LINES);
        glColor3f(0.2, 0.1, 0.3);
        glVertex3i(x1, y1, z1);
        glColor3f(0.2, 0.1, 0.3);
        glVertex3i(x2, y2, z2);
        glEnd();
    }

    float getCellSize(){
        int columns = _field->size();
        int rows = _field->begin()->size();
        float blocksz = (float)getMinOGLDimension() / (columns > rows ? columns : rows);
        return blocksz;
    }

    int getMinOGLDimension(){
        return (w < h ? w : h);
    }

public:
    void setField(std::vector<std::vector<bool>>* field){
        _field = field;
        repaint();
    }
};

#endif
