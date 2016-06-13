TEMPLATE = app
INCLUDEPATH += .

TARGET = colorpicker_draw
include(../../examples.pri)

# Input
HEADERS += draw.h drawboard.h
SOURCES += main.cpp draw.cpp drawboard.cpp
