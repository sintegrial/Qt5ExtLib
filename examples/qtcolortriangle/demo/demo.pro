TEMPLATE = app
CONFIG -= moc
INCLUDEPATH += .

TARGET = colortriangle_demo
include(../../examples.pri)

# Input
SOURCES += main.cpp colordialog.cpp sketchpad.cpp
HEADERS += colordialog.h sketchpad.h
