#ifndef OGL_3D
#define OGL_3D
#include <QGLWidget>
#include <QKeyEvent>
#include <iostream>
#include <list>
#include <vector>

class HistoryWidget: public QGLWidget {
    Q_OBJECT
private:
    float angle_x;
    float angle_y;
    float angle_z;
    float z_cube_center;
    int cube_size;
    std::list<std::vector<std::vector<bool>>>* log;

    int last_mouse_x, last_mouse_y;

public:
    HistoryWidget(QWidget* parent = 0): QGLWidget(parent){
        angle_x = 0.0f;
        angle_y = 0.0f;
        angle_z = 0.0f;
        z_cube_center = -250.0f;
        log = nullptr;
        this->setFocusPolicy(Qt::StrongFocus);

        last_mouse_x = 0;
        last_mouse_y = 0;
    }

protected:
    virtual void initializeGL() override {
        glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        glEnable(GL_COLOR_MATERIAL);

        //GLfloat light[] = {.7f, .7f, 0.0f, 1.0f};
        //glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light);

        GLfloat amb_light[] = {0.1f, 0.1f, 0.1f, 1.0f};
        GLfloat diff_light[] = {0.7f, 0.7f, 0.7f, 1.0f};
        glLightfv(GL_LIGHT0, GL_AMBIENT, amb_light);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diff_light);
        glEnable(GL_LIGHT0);
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
            glVertex3i(-1000, -10, -20);
            glVertex3i(1000, -10, -20);
            glVertex3i(0, -10, -1000);
            glVertex3i(0, -10, 1000);
        glEnd();
        */

        glTranslatef(0, 0, z_cube_center);

        GLfloat l0_pos[] = {0, 0, 0, 1.0f};
        glLightfv(GL_LIGHT0, GL_POSITION, l0_pos);

        glRotatef(angle_x, 1.0f, 0.0f, 0.0f);
        glRotatef(angle_y, 0.0f, 1.0f, 0.0f);
        glRotatef(angle_z, 0.0f, 0.0f, 1.0f);
        if(log == nullptr){
            drawRandomMultiCube(200, 10);
        } else {
            drawLog(250);
        }
    }

    virtual void keyPressEvent(QKeyEvent *e) override {
        int k = e->key();
        if(k == Qt::Key_W){
            angle_x += 2.f;
        } else if(k == Qt::Key_S){
            angle_x -= 2.f;
        } else if(k == Qt::Key_A){
            angle_y += 2.f;
        } else if (k == Qt::Key_D){
            angle_y -= 2.f;
        } else if(k == Qt::Key_Q){
            angle_z += 2.f;
        } else if(k == Qt::Key_E){
            angle_z -= 2.f;
        } else if(k == Qt::Key_Up){
            z_cube_center += 5;
        } else if(k == Qt::Key_Down){
            z_cube_center -= 5;
        }
        repaint();
    }

    virtual void mousePressEvent(QMouseEvent *e) override {
        last_mouse_x = e->x();
        last_mouse_y = e->y();
    }

    virtual void mouseMoveEvent(QMouseEvent *e) override {
        int x = e->x();
        int y = e->y();

        if(last_mouse_x != 0 && last_mouse_y != 0){
            angle_y += (x - last_mouse_x) * .1f;
            angle_x += (y - last_mouse_y) * .1f;
        }
        last_mouse_x = x;
        last_mouse_y = y;
        repaint();
    }

private:
    void drawCude(int sz){
        glBegin(GL_QUADS);

        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3i(0, 0, 0);
        glVertex3i(0, sz, 0);
        glVertex3i(sz, sz, 0);
        glVertex3i(sz, 0, 0);

        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3i(0, 0, 0);
        glVertex3i(0, 0, sz);
        glVertex3i(0, sz, sz);
        glVertex3i(0, sz, 0);

        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3i(0, 0, 0);
        glVertex3i(sz, 0, 0);
        glVertex3i(sz, 0, sz);
        glVertex3i(0, 0, sz);

        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3i(sz, sz, sz);
        glVertex3i(sz, sz, 0);
        glVertex3i(0, sz, 0);
        glVertex3i(0, sz, sz);

        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3i(sz, sz, sz);
        glVertex3i(0, sz ,sz);
        glVertex3i(0, 0, sz);
        glVertex3i(sz, 0, sz);

        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3i(sz, sz, sz);
        glVertex3i(sz, 0, sz);
        glVertex3i(sz, 0, 0);
        glVertex3i(sz, sz, 0);        

        glEnd();
    }

    void drawRandomMultiCube(int sz, int n = 20){
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
                        glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
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

    void drawLog(int sz){
        int n = log->begin()->size();
        float block = (float)sz / n;
        int margin = 1;
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glTranslated(-sz / 2.0, sz / 2.0, -sz / 2.0);
        glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
        typedef std::vector<std::vector<bool>> layer;
        float z_coord = sz;
        for(auto it = log->rbegin(); it != log->rend() && z_coord > 0 ; ++it, z_coord -= block){
            layer* l = &(*it);
            glPushMatrix();
            glTranslatef(0, 0, z_coord);
            for(int ix = 0; ix < n; ++ix){
                glPushMatrix();
                glTranslatef(ix * block, 0.0f, 0.0f);
                for(int iy = 0; iy < n; ++iy){
                    glPushMatrix();
                    glTranslatef(0.0f, iy * block, 0.0f);
                    if((*l)[iy][ix]){
                        glColor3ub((int)(ix * block) % 255, (int)(iy * block) % 255, (int)z_coord % 255);
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

public:
    void setLog(std::list<std::vector<std::vector<bool>>>* l){
        log = l;
        repaint();
    }
};

#endif
