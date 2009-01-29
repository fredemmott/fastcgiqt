TEMPLATE = lib
TARGET = FastCgiQt
CONFIG += debug
DEPENDPATH += .
INCLUDEPATH += . ../include/FastCgiQt
QT -= gui
QT += network

# Input
HEADERS += \
	../include/FastCgiQt/DebugHandler.h \
	../include/FastCgiQt/Manager.h \
	../include/FastCgiQt/Request.h \
	../include/FastCgiQt/Responder.h \
	BeginRequestRecord.h \
	EndRequestRecord.h \
	EnumHelpers.h \
	ManagerPrivate.h \
	InputDevice.h \
	OutputDevice.h \
	ParametersRecord.h \
	RecordHeader.h \
	SocketManager.h \
	StandardInputRecord.h \
	StandardOutputRecord.h \
	fastcgi.h \

SOURCES += \
	BeginRequestRecord.cpp \
	DebugHandler.cpp \
	EndRequestRecord.cpp \
	InputDevice.cpp \
	Manager.cpp \
	ManagerPrivate.cpp \
	OutputDevice.cpp \
	ParametersRecord.cpp \
	RecordHeader.cpp \
	Request.cpp \
	Responder.cpp \
	SocketManager.cpp \
	StandardInputRecord.cpp \
	StandardOutputRecord.cpp \

