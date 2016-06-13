TEMPLATE = app

TARGET = mmlwidget
QT += xml

include(../../examples.pri)

HEADERS += mainwindow.h filebrowser.h
SOURCES += main.cpp mainwindow.cpp filebrowser.cpp
RESOURCES += example.qrc
