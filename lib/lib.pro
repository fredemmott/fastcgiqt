TEMPLATE = lib
TARGET = FastCgiQt
CONFIG += debug
DEPENDPATH += .
INCLUDEPATH += . ../include/FastCgiQt
QT -= gui
QT += network

# Input
HEADERS += \
	../include/FastCgiQt/ClientIOInterface.h \
	../include/FastCgiQt/Manager.h \
	../include/FastCgiQt/Request.h \
	../include/FastCgiQt/Responder.h \
	../include/FastCgiQt/Service.h \
	../include/FastCgiQt/ServiceMapper.h \
	BeginRequestRecord.h \
	DebugHandler.h \
	EndRequestRecord.h \
	EnumHelpers.h \
	ManagerPrivate.h \
	InputDevice.h \
	OutputDevice.h \
	ParametersRecord.h \
	RecordHeader.h \
	RequestDataProvider.h \
	SocketManager.h \
	StandardInputRecord.h \
	StandardOutputRecord.h \
	fastcgi.h \

SOURCES += \
	BeginRequestRecord.cpp \
	ClientIOInterface.cpp \
	DebugHandler.cpp \
	EndRequestRecord.cpp \
	InputDevice.cpp \
	Manager.cpp \
	ManagerPrivate.cpp \
	OutputDevice.cpp \
	ParametersRecord.cpp \
	RecordHeader.cpp \
	Request.cpp \
	RequestDataProvider.cpp \
	Responder.cpp \
	Service.cpp \
	ServiceMapper.cpp \
	SocketManager.cpp \
	StandardInputRecord.cpp \
	StandardOutputRecord.cpp \

