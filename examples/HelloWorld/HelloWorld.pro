TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += . ../../include/
QT -= gui

include(../../lib/lib.pri)
QMAKE_LFLAGS_SHAPP += -Wl,-rpath,../../lib
LIBS += -L../../lib

# Input
HEADERS += HelloWorld.h
SOURCES += HelloWorld.cpp main.cpp
