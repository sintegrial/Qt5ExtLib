TEMPLATE = app
DEPENDPATH += .
INCLUDEPATH += .

TARGET = findwidget_text

include(../../examples.pri)

# Input
SOURCES += main.cpp \
           mainwindow.cpp
HEADERS += mainwindow.h

