set(INCROOT ${PROJECT_SOURCE_DIR}/include/Salamandre)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Salamandre)

#FILES
set(FILES_HEADER)
set(FILES_SRC)

add_subdirectory(fiche)
add_subdirectory(crypt)
add_subdirectory(viewer)
add_subdirectory(bin)

