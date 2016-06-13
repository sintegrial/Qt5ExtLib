TEMPLATE = app
INCLUDEPATH += .

TARGET = piemenu_demo

include(../../examples.pri)

# Input
HEADERS += demo.h
SOURCES += main.cpp demo.cpp
RESOURCES += icons.qrc
