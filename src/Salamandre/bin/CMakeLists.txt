#add_executable(toDo main.cc)

add_executable(${CMAKE_PROJECT_NAME} main.cpp ${FILES_HEADER} ${FILES_SRC} ${UIS_HDRS})
QT5_USE_MODULES(${CMAKE_PROJECT_NAME} Widgets OpenGL)
target_link_libraries(${CMAKE_PROJECT_NAME} ${LIBS})
