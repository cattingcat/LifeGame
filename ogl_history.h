#ifndef OGL_HISTORY_H
#define OGL_HISTORY_H
#include <QGLWidget>
#include <QKeyEvent>
#include <GL/glu.h>
#include <iostream>

class HistoryWidget: public QGLWidget{
    Q_OBJECT
private:
    float angle_x;
    float angle_y;
    float angle_z;

    float z;

    int c_size;

public:
    HistoryWidget(QWidget* parent = 0): QGLWidget(parent){
        angle_x = 0.0f;
        angle_y =  0.0f;
        angle_z = 0.0f;
        z = -250.0f;
        this->setFocusPolicy(Qt::StrongFocus);
    }

protected:
    virtual void initializeGL() override {
        glClearColor(0, 0, 0.5f, 1.0f);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_DEPTH_TEST);
    }

    virtual void resizeGL(int w, int h) override {
        glViewport(0, 0, w, h);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glFrustum(-10, 10, -10, 10, 10, 1000);
    }

    virtual void paintGL() override {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

/*
        glColor4ub(1, 100, 1, 50);
        glBegin(GL_LINES);
            glVertex3i(-1000, -10, 0);
            glVertex3i(1000, -10, 0);
            glVertex3i(0, -10, -1000);
            glVertex3i(0, -10, 1000);
        glEnd();
*/

        glTranslatef(0, 0, z);
        glRotatef(angle_x, 1.0f, 0.0f, 0.0f);
        glRotatef(angle_y, 0.0f, 1.0f, 0.0f);
        glRotatef(angle_z, 0.0f, 0.0f, 1.0f);
        drawMultiCube(200, 15);
    }

    virtual void keyPressEvent(QKeyEvent *e) override {
        int k = e->key();
        if(k == Qt::Key_W){
            angle_x += 1.f;
        } else if(k == Qt::Key_S){
            angle_x -= 1.f;
        } else if(k == Qt::Key_A){
            angle_y += 1.f;
        } else if (k == Qt::Key_D){
            angle_y -= 1.f;
        } else if(k == Qt::Key_Q){
            angle_z += 1.f;
        } else if(k == Qt::Key_E){
            angle_z -= 1.f;
        } else if(k == Qt::Key_Up){
            z += 5;
            std::cout<<z << std::endl;
        } else if(k == Qt::Key_Down){
            z -= 5;
        }
        repaint();
    }

    virtual void mousePressEvent(QMouseEvent *e) override {
    }

private:
    void drawCude(int sz){
        glBegin(GL_QUADS);

        glVertex3i(0, 0, 0);
        glVertex3i(0, sz, 0);
        glVertex3i(sz, sz, 0);
        glVertex3i(sz, 0, 0);

        glVertex3i(0, 0, 0);
        glVertex3i(0, 0, sz);
        glVertex3i(0, sz, sz);
        glVertex3i(0, sz, 0);

        glVertex3i(0, 0, 0);
        glVertex3i(sz, 0, 0);
        glVertex3i(sz, 0, sz);
        glVertex3i(0, 0, sz);

        glVertex3i(sz, sz, sz);
        glVertex3i(sz, sz, 0);
        glVertex3i(0, sz, 0);
        glVertex3i(0, sz, sz);

        glVertex3i(sz, sz, sz);
        glVertex3i(0, sz ,sz);
        glVertex3i(0, 0, sz);
        glVertex3i(sz, 0, sz);

        glVertex3i(sz, sz, sz);
        glVertex3i(sz, 0, sz);
        glVertex3i(sz, 0, 0);
        glVertex3i(sz, sz, 0);        

        glEnd();
    }

    void drawMultiCube(int sz, int n = 20){
        int margin = 1;
        float block = sz / n;
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glTranslated(-sz / 2.0, -sz / 2.0, -sz / 2.0);
        for(float ix = 0; ix < sz; ix += block){
            glPushMatrix();
            glTranslated(ix, 0, 0);
            for(float iy = 0; iy < sz; iy += block){
                glPushMatrix();
                glTranslated(0, iy, 0);
                for(float iz = 0; iz < sz; iz += block){
                    glPushMatrix();
                    glTranslated(0, 0, iz);
                    if(qrand() % 2){
                        glColor3ub((int)ix % 255, (int)iy % 255, (int)iz % 255);
                        drawCude(block - margin);
                    }
                    glPopMatrix();
                }
                glPopMatrix();
            }
            glPopMatrix();
        }
        glPopMatrix();
    }

};

#endif
