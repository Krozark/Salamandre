set(INCROOT ${PROJECT_SOURCE_DIR}/include/Salamandre/viewer)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Salamandre/viewer)

QT5_WRAP_UI(UIS_HDRS
    gui/mainwindow.ui
)

add_subdirectory(gui)
add_subdirectory(shared)

# append in local scope
list(APPEND FILES_HEADER
)
# set in parent scope
set(FILES_HEADER "${FILES_HEADER}" PARENT_SCOPE)

list(APPEND FILES_SRC
)
set(FILES_SRC "${FILES_SRC}" PARENT_SCOPE)

SET(QtApp_RCCS ${CMAKE_PROJECT_NAME}.qrc)
