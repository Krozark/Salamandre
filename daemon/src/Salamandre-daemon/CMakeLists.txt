set(INCROOT ${PROJECT_SOURCE_DIR}/include/${CMAKE_PROJECT_NAME})
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/${CMAKE_PROJECT_NAME})

configure_file(${INCROOT}/define.hpp.in ${INCROOT}/define.hpp @ONLY)

#ROOT files
set(ROOT_SRC
    ${SRCROOT}/Daemon.cpp
    ${SRCROOT}/FileManager.cpp
    ${SRCROOT}/FileInfo.cpp
    ${SRCROOT}/GuiFunctions.cpp
    ${SRCROOT}/ServerBroadcast
    ${SRCROOT}/ServerFunctions.cpp
)
set(ROOT_HEADER
    ${INCROOT}/Daemon.hpp
    ${INCROOT}/FileManager.hpp
    ${INCROOT}/FileInfo.hpp
    ${INCROOT}/GuiFunctions.hpp
    ${INCROOT}/GuiFunctions.tpl
    ${INCROOT}/ServerBroadcast.hpp
    ${INCROOT}/ServerFunctions.hpp
)
set(ROOT_FILES
    ${ROOT_HEADER}
    ${ROOT_SRC}
)

#FILES
set(FILES_HEADER
    ${ROOT_HEADER}
)
set(FILES_SRC
    ${ROOT_SRC}
)

#SHARED lib
add_library(${CMAKE_PROJECT_NAME} SHARED ${FILES_HEADER} ${FILES_SRC})
SET_TARGET_PROPERTIES(${CMAKE_PROJECT_NAME}
    PROPERTIES
    VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
    SOVERSION ${VERSION_MAJOR}.${VERSION_MINOR}
)
target_link_libraries(${CMAKE_PROJECT_NAME} ${LIBS})
INSTALL(TARGETS ${CMAKE_PROJECT_NAME}
    DESTINATION lib
)

#static lib
#if(UNIX)
add_library(${CMAKE_PROJECT_NAME}-static STATIC ${FILES_HEADER} ${FILES_SRC})
SET_TARGET_PROPERTIES(${CMAKE_PROJECT_NAME}-static
	PROPERTIES
	VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
	SOVERSION ${VERSION_MAJOR}.${VERSION_MINOR}
)
target_link_libraries(${CMAKE_PROJECT_NAME}-static ${LIBS})
INSTALL(TARGETS ${CMAKE_PROJECT_NAME}-static
	DESTINATION lib
)

add_executable(${DAEMON_NAME} main.cpp ${FILES_HEADER} ${FILES_SRC})
target_link_libraries(${DAEMON_NAME} ${LIBS})
