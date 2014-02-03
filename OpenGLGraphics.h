#ifndef OPENGLGRAPHICS_H
#define OPENGLGRAPHICS_H
#include <QGLWidget>
#include <QWheelEvent>
#define M_PI 3.14159265358979323846
#include <iostream>

class OpenGLGraphics: public QGLWidget
{
    Q_OBJECT
private:
    int w_window;
    int h_window;
    int w;
    int h;
    std::vector<std::vector<bool>>* _field;

public:
    OpenGLGraphics(QWidget* parent = 0): QGLWidget(parent), _field(nullptr){
        w = 1000;
        h = 1000;
    }


protected:
    virtual void initializeGL(){
        glClearColor(0, 0, 0, 0);
    }

    virtual void resizeGL(int w_w, int h_w){
        w_window = w_w;
        h_window = h_w;
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glViewport(0, 0, w_window, h_window);
        glOrtho(0, w, h, 0, -1, 1);
    }

    virtual void paintGL(){
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        if(_field != nullptr && !_field->empty()){
            int n = _field->size();
            int blocksz = w / n;
            int cube_size = (double)blocksz * 3.0 / 4.0;
            int cube_margin = blocksz - cube_size;

            for(int i = 0; i < n; ++i){
                for(int j = 0; j < n; ++j){
                    int x = blocksz * i;
                    int y = blocksz * j;
                    if((*_field)[i][j])
                        drawSquare(x, y, 0, cube_size);
                }
            }
        }
        //glFinish();
    }

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

public:
    void setField(std::vector<std::vector<bool>>* field){
        _field = field;
        repaint();
    }
};

#endif
