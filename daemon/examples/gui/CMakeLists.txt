set(HEADERS
    defines.hpp
    functions.hpp
)

set(SRC
    client.cpp
    functions.cpp
)

set(FILES ${HEADERS} ${SRC})

add_executable(gui-client ${FILES})
target_link_libraries(gui-client ${LIBS} ${CMAKE_PROJECT_NAME})
