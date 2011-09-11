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
target_link_libraries(la ${LIBS})

set(la_LIB la PARENT_SCOPE)
