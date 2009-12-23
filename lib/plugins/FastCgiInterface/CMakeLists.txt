SET(
	CLASSES
	BeginRequestRecord
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
SET(HEADERS SocketServer.h)

IF(UNIX)
	OPTION(WITH_UNIX_SOCKET_SUPPORT "Enable support for FastCGI over unix sockets" ON)
	IF(WITH_UNIX_SOCKET_SUPPORT)
		OPTION(WITH_SYSLOG_SUPPORT "Use syslog for debug/error messages with FCGI-UNIX" OFF)
		MESSAGE(STATUS "FCGI-UNIX (FastCGI over unix sockets) is enabled.")
	ELSE()
		MESSAGE(STATUS "FCGI-UNIX (FastCGI over unix sockets) is *NOT* enabled.")
	ENDIF()
	IF(WITH_SYSLOG_SUPPORT)
		LIST(APPEND CLASSES DebugHandler)
		ADD_DEFINITIONS(-DWITH_SYSLOG_SUPPORT)
		MESSAGE(STATUS "FCGI-UNIX debugging via syslog is enabled.")
	ELSE()
		MESSAGE(STATUS "FCGI-UNIX debugging via syslog is *NOT* enabled.")
	ENDIF()
ELSE()
	SET(WITH_UNIX_SOCKET_SUPPORT OFF)
ENDIF()

FOREACH(CLASS ${CLASSES})
	LIST(APPEND SOURCES ${CLASS}.cpp)
	LIST(APPEND HEADERS ${CLASS}.h)
ENDFOREACH()

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
