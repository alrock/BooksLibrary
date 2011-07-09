#-------------------------------------------------
#
# Project created by QtCreator 2011-06-18T21:20:05
#
#-------------------------------------------------

QT       += core gui webkit script declarative

TARGET = BooksLibrary
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    googlevolume.cpp \
    googlesearchengine.cpp \
    volume.cpp \
    thumbnailloader.cpp \
    qmltestview.cpp \
    shelf.cpp \
    shelfview.cpp \
    volumerenderer.cpp

HEADERS  += mainwindow.h \
    googlevolume.h \
    googlesearchengine.h \
    volume.h \
    thumbnailloader.h \
    qmltestview.h \
    shelf.h \
    shelfview.h \
    volumerenderer.h

OTHER_FILES += \
    test.qml \
    TestDelegate.qml
