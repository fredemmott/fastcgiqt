SET(
	PUBLIC_CLASSES
	ClientIOInterface
	Manager
	Request
	Responder
	Service
	ServiceMapper
)

SET(
	PRIVATE_CLASSES
	BeginRequestRecord
	Cache
	DebugHandler
	EndRequestRecord
	EnumHelpers
	FileCache
	FileDependentCache
	InputDevice
	ManagerPrivate
	OutputDevice
	ParametersRecord
	RecordHeader
	RequestDataProvider
	SocketManager
	StandardInputRecord
	StandardOutputRecord
)

SET(VERSION 0.0.1)

INCLUDE_DIRECTORIES(../include/FastCgiQt)

##### Probably don't want to edit below this line ##### 
SET(PUBLIC_HEADERS)

FOREACH(class ${PRIVATE_CLASSES})
	SET(SOURCES ${SOURCES} ${class}.cpp)
	SET(HEADERS ${HEADERS} ${class}.h)
ENDFOREACH()

FOREACH(class ${PUBLIC_CLASSES})
	SET(SOURCES ${SOURCES} ${class}.cpp)
	SET(HEADERS ${HEADERS} ../include/FastCgiQt/${class}.h)

	SET(PUBLIC_HEADERS ${PUBLIC_HEADERS} ../include/FastCgiQt/${class}.h)
ENDFOREACH()

QT4_WRAP_CPP(MOC_SOURCES ${HEADERS})

ADD_LIBRARY(
	FastCgiQt
	SHARED
	${BUILD}
	${SOURCES}
	${MOC_SOURCES}
)

SET_TARGET_PROPERTIES(
	FastCgiQt
	PROPERTIES
	VERSION "${VERSION}"
	SOVERSION "0"
)

TARGET_LINK_LIBRARIES(
	FastCgiQt
	${QT_LIBRARIES}
)

INSTALL(
	TARGETS
	FastCgiQt
	DESTINATION
	lib${LIB_SUFFIX}
)

INSTALL(
	FILES
	${PUBLIC_HEADERS}
	DESTINATION
	include/FastCgiQt/
)
