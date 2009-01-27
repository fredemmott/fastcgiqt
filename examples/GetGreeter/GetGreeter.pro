TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += . ../../include/
QT -= gui

include(../../lib/lib.pri)
LIBS += -L../../lib
QMAKE_RPATHDIR += $$(PWD)/../../lib/

# Input
HEADERS += GetGreeter.h
SOURCES += GetGreeter.cpp main.cpp
