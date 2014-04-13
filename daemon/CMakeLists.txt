cmake_minimum_required (VERSION 2.8.8)

# define a macro that helps defining an option
macro(set_option var default type docstring)
    if(NOT DEFINED ${var})
        set(${var} ${default})
    endif()
    set(${var} ${${var}} CACHE ${type} ${docstring} FORCE)
endmacro()

# project name
project (Salamandre-daemon)

# setup version numbers
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)


# add an option for choosing the build type (shared or static)
#set_option(BUILD_SHARED_LIBS FALSE BOOL "TRUE to build as shared libraries, FALSE to build it as static libraries")

# add an option for building the API documentation
set_option(BUILD_DOC TRUE BOOL "TRUE to generate the API documentation, FALSE to ignore it")

# add an option for building the API examples
set_option(BUILD_EXAMPLES TRUE BOOL "TRUE to generate the exmamples, FALSE to ignore it")

find_library(SOCKET_LIB socket)
find_library(STATS_LIB Salamandre-stats)
find_library(ORM_LIB orm)
find_library(SQLITE_LIB sqlite3)
set(LIBS ${SOCKET_LIB} ${STATS_LIB} ${ORM_LIB}
    ${SQLITE_LIB})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
set(DEFINES "")
set(WARNING "-Wall")
set(FLAGS "-o3 -std=c++0x")
set(CMAKE_CXX_FLAGS "${DEFINES} ${WARNING} ${FLAGS}")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(LIB_INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# add the subdirectories
add_subdirectory(src/${CMAKE_PROJECT_NAME})
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
#build doc
if(BUILD_DOC)
    add_subdirectory(doc)
endif()

