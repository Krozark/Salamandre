set(INCROOT ${PROJECT_SOURCE_DIR}/examples)
set(SRCROOT ${PROJECT_SOURCE_DIR}/examples)

add_executable(test test.cpp)
target_link_libraries(test ${LIBS} ${CMAKE_PROJECT_NAME})


