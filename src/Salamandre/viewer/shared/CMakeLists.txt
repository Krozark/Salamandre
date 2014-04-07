set(INCROOT ${PROJECT_SOURCE_DIR}/include/Salamandre/viewer/shared)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Salamandre/viewer/shared)

# append in local scope
list(APPEND FILES_HEADER
    ${INCROOT}/graphicsscene.hpp
    ${INCROOT}/graphicsitemcomputer.hpp 
    ${INCROOT}/graphicsitemlink.hpp
)
# set in parent scope
set(FILES_HEADER "${FILES_HEADER}" PARENT_SCOPE)

list(APPEND FILES_SRC
    ${SRCROOT}/graphicsscene.cpp 
    ${SRCROOT}/graphicsitemcomputer.cpp
    ${SRCROOT}/graphicsitemlink.cpp
)

set(FILES_SRC "${FILES_SRC}" PARENT_SCOPE)
