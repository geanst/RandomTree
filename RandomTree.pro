#-------------------------------------------------
#
# Project created by QtCreator 2019-06-05T12:00:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RandomTree
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cell.cpp \
    grid.cpp \
    rrt.cpp

HEADERS  += mainwindow.h \
    cell.h \
    grid.h \
    rrt.h

FORMS    += mainwindow.ui
