cmake_minimum_required(VERSION 2.8)
project(la)

include(CheckLibraryExists)

find_package(Lua51 REQUIRED)
find_package(portaudio REQUIRED)

set(LIBS lua5.1)

if (WIN32)
    set(LIBS ${LIBS} portaudio_x86)
else (WIN32)
    set(LIBS ${LIBS} portaudio)
endif()

file(GLOB_RECURSE la_SOURCES "${la_SOURCE_DIR}/*.c*")

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-Wno-unused-value")
endif()

include_directories(${la_SOURCE_DIR} ${LUA_INCLUDE_DIR} ${Portaudio_INCLUDE_DIR})
link_directories(${LUA_LIBRARY_DIR} ${Portaudio_LIBRARY_DIR})

message(STATUS ${Portaudio_INCLUDE_DIR})
message(STATUS ${LUA_INCLUDE_DIR})
message(STATUS ${Portaudio_LIBRARY_DIR})
message(STATUS ${LUA_LIBRARY_DIR})

add_library(la MODULE ${la_SOURCES})
set_target_properties(la 
    PROPERTIES 
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY "../../bin"
)
target_link_libraries(la ${LIBS})

add_library(la-s STATIC ${la_SOURCES})
set_target_properties(la-s 
    PROPERTIES 
    PREFIX ""
    ARCHIVE_OUTPUT_DIRECTORY "../../bin"
)
target_link_libraries(la-s ${LIBS})

set(la_LIB la PARENT_SCOPE)
