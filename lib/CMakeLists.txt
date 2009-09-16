SET(
	PUBLIC_CLASSES
	ClientIOInterface
	Cache
	CacheEntry
	Manager
	Request
	Responder
)

SET(
	PRIVATE_CLASSES
	BeginRequestRecord
	CacheBackend
	CacheEntryPrivate
	CachePrivate
	Caches
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
	UnsignedByteArray
)

IF(WITH_SERVICE_SUPPORT)
	LIST(
		APPEND
		PUBLIC_CLASSES
		Service
		ServiceMapper
	)
	LIST(
		APPEND
		PRIVATE_CLASSES
		ServiceMapperPrivate
	)
ENDIF()

IF(WITH_SQL_SUPPORT)
	LIST(
		APPEND
		PUBLIC_CLASSES
		Database
	)
	LIST(
		APPEND
		PRIVATE_CLASSES
		DatabaseGuard
	)
ENDIF()

IF(WITH_XSLT_SUPPORT)
	LIST(
		APPEND
		PUBLIC_CLASSES
		XsltService
	)
ENDIF()

SET(VERSION 0.0.1)


##### Probably don't want to edit below this line ##### 
INCLUDE_DIRECTORIES(../include/FastCgiQt ${CMAKE_CURRENT_BINARY_DIR})
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

ADD_SUBDIRECTORY(plugins)

SET(EXTRA_LIBRARIES)

IF(WITH_MEMCACHED_SUPPORT)
	FIND_LIBRARY(LIBMEMCACHED memcached)
	IF(${LIBMEMCACHED} STREQUAL LIBMEMCACHED-NOTFOUND)
		MESSAGE(FATAL_ERROR "memcached support was enabled, but libmemcached could not be found.")
	ENDIF()
	LIST(APPEND EXTRA_LIBRARIES ${LIBMEMCACHED})
ENDIF()

SET(STATIC_PLUGINS_H ${CMAKE_CURRENT_BINARY_DIR}/plugins/config.h)
FILE(WRITE ${STATIC_PLUGINS_H} "#include <QtPlugin>\n")
FOREACH(Plugin ${STATIC_PLUGINS})
	FILE(APPEND ${STATIC_PLUGINS_H} "Q_IMPORT_PLUGIN(FastCgiQt_${Plugin});\n")
ENDFOREACH()

INCLUDE(AddFileDependencies)
ADD_FILE_DEPENDENCIES(Cache.cpp ${STATIC_PLUGINS_H})

TARGET_LINK_LIBRARIES(
	FastCgiQt
	${QT_LIBRARIES}
	${STATIC_PLUGINS}
	${EXTRA_LIBRARIES}
)

