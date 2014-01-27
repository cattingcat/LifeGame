#include <QThread>
#include "graphicswidget.h"
#include "threadhelper.h"

ThreadHelper::ThreadHelper(GraphicsWidget *gw){
    mgw = gw;
}

void ThreadHelper::run(){
    if(mgw->log.size() < mgw->max_log_length){
        mgw->log.push_back(mgw->field);
    } else {
        mgw->log.pop_front();
        mgw->log.push_back(mgw->field);
    }
    mgw->field = mgw->createField();
    mgw->calculateNext();
}

