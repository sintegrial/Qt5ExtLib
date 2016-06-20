include(../common.pri)
INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

qtimagefilters-uselib:!qtimagefilters-buildlib {
    LIBS += -L$$QTIMAGEFILTERS_LIBDIR -l$$QTIMAGEFILTERS_LIBNAME
} else {
    SOURCES += convolutionfilter.cpp qtimagefilter.cpp punchfilter.cpp qtimagefilterfactory.cpp
    HEADERS += qtimagefilter.h convolutionfilter.h gaussfilter.h punchfilter.h qtmatrix.h
}

win32 {
    contains(TEMPLATE, lib):contains(CONFIG, shared):DEFINES += QT_QTIMAGEFILTERS_EXPORT
    else:qtimagefilters-uselib:DEFINES += QT_QTIMAGEFILTERS_IMPORT
}
