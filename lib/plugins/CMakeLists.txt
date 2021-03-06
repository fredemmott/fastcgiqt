SET(
	STATIC_PLUGINS
	RamCache
)

SET(SUBDIR_STATIC_PLUGINS)

IF(WITH_FASTCGI_SUPPORT)
	LIST(APPEND SUBDIR_STATIC_PLUGINS FastCgiInterface)
	MESSAGE(STATUS "FastCGI support is enabled.")
ELSE()
	MESSAGE(STATUS "FastCGI support is *NOT* enabled.")
ENDIF()

IF(WITH_SCGI_SUPPORT)
	LIST(APPEND SUBDIR_STATIC_PLUGINS ScgiInterface)
	MESSAGE(STATUS "SCGI support is enabled.")
ELSE()
	MESSAGE(STATUS "SCGI support is *NOT* enabled.")
ENDIF()

IF(WITH_HTTP_SUPPORT)
	LIST(APPEND SUBDIR_STATIC_PLUGINS HttpInterface)
	MESSAGE(STATUS "HTTP support is enabled.")
ELSE()
	MESSAGE(STATUS "HTTP support is *NOT* enabled.")
ENDIF()

IF(WITH_CGI_SUPPORT)
	LIST(APPEND SUBDIR_STATIC_PLUGINS CgiInterface)
	MESSAGE(STATUS "CGI support is enabled.")
ELSE()
	MESSAGE(STATUS "CGI support is *NOT* enabled.")
ENDIF()

IF(WITH_MEMCACHED_SUPPORT)
	LIST(APPEND STATIC_PLUGINS MemcachedCache)
	MESSAGE(STATUS "memcached support is enabled")
ELSE()
	MESSAGE(STATUS "memcached support is *NOT* enabled.")
ENDIF()

# Probably don't want to edit below here


ADD_DEFINITIONS(-fPIC)
INCLUDE_DIRECTORIES(.. ../../include/FastCgiQt ${CMAKE_CURRENT_BINARY_DIR})

FOREACH(plugin ${STATIC_PLUGINS})
	SET(MOC_SOURCES)
	SET(SOURCES ${plugin}.cpp)
	QT4_WRAP_CPP(MOC_SOURCES ${plugin}.h)
	ADD_LIBRARY(
		${plugin}
		${SOURCES}
		${MOC_SOURCES}
	)
ENDFOREACH()

FOREACH(subdir ${SUBDIR_STATIC_PLUGINS})
	ADD_SUBDIRECTORY(${subdir})
ENDFOREACH()

SET(
	STATIC_PLUGINS
	${STATIC_PLUGINS}
	${SUBDIR_STATIC_PLUGINS}
	PARENT_SCOPE
)
