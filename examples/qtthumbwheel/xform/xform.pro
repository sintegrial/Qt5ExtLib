TEMPLATE = app
INCLUDEPATH += .

TARGET = thumbwheel_xform

include(../../examples.pri)

# Input
SOURCES += main.cpp
RESOURCES = xform.qrc

unix {
    MOC_DIR=.moc
    OBJECTS_DIR=.obj
}
