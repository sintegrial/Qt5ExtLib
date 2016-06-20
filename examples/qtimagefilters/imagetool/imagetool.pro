TEMPLATE = app
#CONFIG -= moc
INCLUDEPATH += .
QT += core

include(../../examples.pri)

# Input
SOURCES += main.cpp imagetoolwidget.cpp
HEADERS += imagetoolwidget.h mirrorfilter.h

FORMS   += imagetoolwidget.ui
