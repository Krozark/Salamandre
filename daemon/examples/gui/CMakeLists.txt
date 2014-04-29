
set(HEADERS
    functions.hpp
    ${PROJECT_SOURCE_DIR}/include/${CMAKE_PROJECT_NAME}/std.hpp
    ${PROJECT_SOURCE_DIR}/include/${CMAKE_PROJECT_NAME}/std.tpl
    ${PROJECT_SOURCE_DIR}/include/${CMAKE_PROJECT_NAME}/GuiFunctions.hpp
    ${PROJECT_SOURCE_DIR}/include/${CMAKE_PROJECT_NAME}/GuiFunctions.tpl
)

set(SRC
    client.cpp
    functions.cpp
    ${PROJECT_SOURCE_DIR}/src/${CMAKE_PROJECT_NAME}/std.cpp

)

set(FILES ${HEADERS} ${SRC})

add_executable(gui-client ${FILES})
target_link_libraries(gui-client ${LIBS})
