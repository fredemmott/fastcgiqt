TEMPLATE = lib
TARGET = FastCgiQt
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
	OutputDevice.h \
	ParametersRecord.h \
	RecordHeader.h \
	StandardInputRecord.h \
	StandardOutputRecord.h \
	fastcgi.h \

SOURCES += \
	BeginRequestRecord.cpp \
	DebugHandler.cpp \
	EndRequestRecord.cpp \
	Manager.cpp \
	ManagerPrivate.cpp \
	OutputDevice.cpp \
	ParametersRecord.cpp \
	RecordHeader.cpp \
	Request.cpp \
	Responder.cpp \
	StandardInputRecord.cpp \
	StandardOutputRecord.cpp \
