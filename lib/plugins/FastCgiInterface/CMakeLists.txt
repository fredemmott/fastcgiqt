SET(
	CLASSES
	BeginRequestRecord
	DebugHandler
	EndRequestRecord
	FastCgiInterface
	FastCgiInterfaceFactory
	FastCgiSocketManager
	FastCgiStream
	ParametersRecord
	RecordHeader
	StandardInputRecord
	StandardOutputRecord
	UnsignedByteArray
)

SET(SOURCES)
SET(HEADERS)

FOREACH(CLASS ${CLASSES})
	LIST(APPEND SOURCES ${CLASS}.cpp)
	LIST(APPEND HEADERS ${CLASS}.h)
ENDFOREACH()

LIST(APPEND HEADERS SocketServer.h)
IF(UNIX)
	OPTION(WITH_UNIX_SOCKET_SUPPORT "Enable support for FastCGI over unix sockets" ON)
ELSE()
	SET(WITH_UNIX_SOCKET_SUPPORT OFF)
ENDIF()
IF(WITH_UNIX_SOCKET_SUPPORT)
	LIST(APPEND SOURCES SocketServer_unix.cpp)
ELSE()
	LIST(APPEND SOURCES SocketServer_tcp.cpp)
ENDIF()

SET(MOC_SOURCES)
QT4_WRAP_CPP(MOC_SOURCES ${HEADERS})

INCLUDE_DIRECTORIES(../.. ../../../include/FastCgiQt ${CMAKE_CURRENT_BINARY_DIR})

ADD_LIBRARY(
	FastCgiInterface
	${SOURCES}
	${MOC_SOURCES}
)
