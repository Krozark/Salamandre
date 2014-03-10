set(INCROOT ${PROJECT_SOURCE_DIR}/include/Salamandre)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Salamandre)



#FILES
set(FILES_HEADER)
set(FILES_SRC)

add_subdirectory(fiche)
add_subdirectory(crypt)

#SHARED lib
add_library(salamandre SHARED ${FILES_HEADER} ${FILES_SRC})
SET_TARGET_PROPERTIES(salamandre
    PROPERTIES
    VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
    SOVERSION ${VERSION_MAJOR}.${VERSION_MINOR}
)
INSTALL(TARGETS salamandre
    DESTINATION lib
)

#static lib
add_library(salamandre-static STATIC ${FILES_HEADER} ${FILES_SRC})
SET_TARGET_PROPERTIES(salamandre-static
    PROPERTIES
    VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
    SOVERSION ${VERSION_MAJOR}.${VERSION_MINOR}
)
INSTALL(TARGETS salamandre-static
    DESTINATION lib
)

#header
#INSTALL(FILES ${INCROOT}
#    DESTINATION include
#    PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
#)

file(GLOB DEPLOY_FILES_AND_DIRS "${PROJECT_SOURCE_DIR}/include/*")
foreach(ITEM ${DEPLOY_FILES_AND_DIRS})
   IF( IS_DIRECTORY "${ITEM}" )
      LIST( APPEND DIRS_TO_DEPLOY "${ITEM}" )
   ELSE()
      LIST( APPEND FILES_TO_DEPLOY "${ITEM}" )
   ENDIF()
endforeach()
INSTALL( FILES ${FILES_TO_DEPLOY} DESTINATION include )
INSTALL( DIRECTORY ${DIRS_TO_DEPLOY} DESTINATION include )


########################################################################
# Create uninstall target
########################################################################
configure_file(
    ${CMAKE_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
@ONLY)
add_custom_target(uninstall
    ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
)
########################################################################
