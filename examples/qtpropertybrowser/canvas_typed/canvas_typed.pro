TEMPLATE = app
DEPENDPATH += .
INCLUDEPATH += .

TARGET = property_canvas_typed

include(../../examples.pri)

# Input
HEADERS += qtcanvas.h mainwindow.h
SOURCES += qtcanvas.cpp mainwindow.cpp main.cpp

