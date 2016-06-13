TEMPLATE = lib

CONFIG += static
#CONFIG += c++11
QT += core gui widgets

CONFIG(debug, debug|release){
	TARGET = Qt5ExtLibd
} else {
	TARGET = Qt5ExtLib
}

DESTDIR = $$OUT_PWD/../lib

# widgets
SOURCES += $$files($$PWD/qtwidgets/*.cpp)
HEADERS += $$files($$PWD/qtwidgets/*.h)
FORMS += $$files($$PWD/qtwidgets/*.ui)
RESOURCES += $$files($$PWD/qtwidgets/*.qrc)

# property browser
SOURCES += $$files($$PWD/qtpropertybrowser/*.cpp)
HEADERS += $$files($$PWD/qtpropertybrowser/*.h)
FORMS += $$files($$PWD/qtpropertybrowser/*.ui)
RESOURCES += $$files($$PWD/qtpropertybrowser/*.qrc)

# gradient editor
SOURCES += $$files($$PWD/qtgradienteditor/*.cpp)
HEADERS += $$files($$PWD/qtgradienteditor/*.h)
FORMS += $$files($$PWD/qtgradienteditor/*.ui)
RESOURCES += $$files($$PWD/qtgradienteditor/*.qrc)
INCLUDEPATH += $$PWD/qtgradienteditor

# font panel
SOURCES += $$files($$PWD/fontpanel/*.cpp)
HEADERS += $$files($$PWD/fontpanel/*.h)

# find widget
SOURCES += $$files($$PWD/findwidget/*.cpp)
HEADERS += $$files($$PWD/findwidget/*.h)
RESOURCES += $$files($$PWD/findwidget/*.qrc)


