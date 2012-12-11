# 
# CS123 Final Project Starter Code
# Adapted from starter code graciously provided by CS195-U: 3D Game Engines
#

QT += core gui opengl

TARGET = final
TEMPLATE = app

# If you add your own folders, add them to INCLUDEPATH and DEPENDPATH, e.g.
# INCLUDEPATH += folder1 folder2
# DEPENDPATH += folder1 folder2

SOURCES += main.cpp \
    mainwindow.cpp \
    view.cpp \
    settings.cpp \
    camera.cpp \
    bottom_terrain.cpp

HEADERS += mainwindow.h \
    view.h \
    camera.h \
    settings.h \
    bottom_terrain.h

FORMS += mainwindow.ui
