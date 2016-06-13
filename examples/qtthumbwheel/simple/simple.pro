TEMPLATE = app
INCLUDEPATH += .

TARGET = thumbwheel_simple

include(../../examples.pri)

# Input
SOURCES += main.cpp
unix {
    OBJECTS_DIR=.obj
    MOC_DIR = .moc
}
