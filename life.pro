#-------------------------------------------------
#
# Project created by QtCreator 2014-01-27T12:54:06
#
#-------------------------------------------------

QT       += core gui
QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = life
TEMPLATE = app


SOURCES += main.cpp \
    threadhelper.cpp \
    graphicswidget.cpp

HEADERS  += mainwindow.h \
    graphicswidget.h \
    threadhelper.h

FORMS    += mainwindow.ui
