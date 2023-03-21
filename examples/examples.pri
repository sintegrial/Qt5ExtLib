#CONFIG += c++11
QT += core gui widgets

INCLUDEPATH += $$PWD/../src/qtwidgets
INCLUDEPATH += $$PWD/../src/qtpropertybrowser $$PWD/../src/qtpropertybrowserext
INCLUDEPATH += $$PWD/../src/qtgradienteditor
INCLUDEPATH += $$PWD/../src/qtimagefilters
INCLUDEPATH += $$PWD/../src/findwidget
INCLUDEPATH += $$PWD/../src/qtsingleapplication

DESTDIR = $$OUT_PWD/../../../examples

LIBS += -L$$OUT_PWD/../../../lib

CONFIG(debug, debug|release){
	LIBS += -lQt5ExtLibd
} else {
	LIBS += -lQt5ExtLib
}

