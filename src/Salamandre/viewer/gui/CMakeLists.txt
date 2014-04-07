set(INCROOT ${PROJECT_SOURCE_DIR}/include/Salamandre/viewer/gui)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Salamandre/viewer/gui)


# append in local scope
list(APPEND FILES_HEADER
    ${INCROOT}/mainwindow.hpp
)
# set in parent scope
set(FILES_HEADER "${FILES_HEADER}" PARENT_SCOPE)

list(APPEND FILES_SRC
    ${SRCROOT}/mainwindow.cpp
)
set(FILES_SRC "${FILES_SRC}" PARENT_SCOPE)

