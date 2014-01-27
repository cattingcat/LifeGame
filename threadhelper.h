#ifndef THREADHELPER_H
#define THREADHELPER_H
#include "graphicswidget.h"
#include <QThread>
class ThreadHelper;

class ThreadHelper : public QThread{
    Q_OBJECT
private:
    GraphicsWidget *mgw;
public:
    ThreadHelper(GraphicsWidget *gw);
private:
    void run();
};
#endif
