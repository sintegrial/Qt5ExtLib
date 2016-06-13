TEMPLATE = app
DEPENDPATH += .
INCLUDEPATH += .

TARGET = toolbardialog

include(../../examples.pri)

# Input
SOURCES += main.cpp \
           mainwindow.cpp
HEADERS += mainwindow.h
RESOURCES += simple.qrc

