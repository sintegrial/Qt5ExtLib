TEMPLATE = app
DEPENDPATH += .
INCLUDEPATH += .

TARGET = property_object_controller

include(../../examples.pri)

# Input
HEADERS += objectcontroller.h
SOURCES += objectcontroller.cpp main.cpp

