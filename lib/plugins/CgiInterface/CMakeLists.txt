SET(
	CLASSES
	CgiInterface
	CgiInterfaceFactory
	CgiRequest
)

SET(SOURCES)
SET(HEADERS)

FOREACH(CLASS ${CLASSES})
	LIST(APPEND SOURCES ${CLASS}.cpp)
	LIST(APPEND HEADERS ${CLASS}.h)
ENDFOREACH()

SET(MOC_SOURCES)
QT4_WRAP_CPP(MOC_SOURCES ${HEADERS})

INCLUDE_DIRECTORIES(../.. ../../../include/FastCgiQt ${CMAKE_CURRENT_BINARY_DIR})

ADD_LIBRARY(
	CgiInterface
	${SOURCES}
	${MOC_SOURCES}
)
