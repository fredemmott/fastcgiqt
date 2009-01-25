TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += . ../../include/
QT -= gui

include(../../lib/lib.pri)
LIBS += -L../../lib
QMAKE_RPATHDIR += $$(PWD)/../../lib/

# Input
HEADERS += HelloWorld.h
SOURCES += HelloWorld.cpp main.cpp
