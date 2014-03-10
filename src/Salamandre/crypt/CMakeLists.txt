set(INCROOT ${PROJECT_SOURCE_DIR}/include/Salamandre/crypt)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Salamandre/crypt)
# append in local scope
list(APPEND FILES_HEADER
)
# set in parent scope
set(FILES_HEADER "${FILES_HEADER}" PARENT_SCOPE)

list(APPEND FILES_SRC
)

# set in parent scope
set(FILES_SRC "${FILES_SRC}" PARENT_SCOPE)
