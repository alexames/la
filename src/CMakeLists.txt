cmake_minimum_required (VERSION 2.8)
project(laroot)

set(CMAKE_MODULE_PATH ${laroot_SOURCE_DIR}/../cmake ${CMAKE_MODULE_PATH})

include(MacroOutOfSourceBuild)
    macro_ensure_out_of_source_build("${PROJECT_NAME} requires an out of source build.")

set(BASE_RELEASE_DIR ${laroot_SOURCE_DIR}/..)

if (WIN32)
    set(CUSTOM_CFLAGS ${CUSTOM_CFLAGS} -DNOMINMAX -D_CRT_SECURE_NO_WARNINGS)
endif()

if (CMAKE_COMPILER_IS_GNUCXX)
    set(CUSTOM_CFLAGS ${CUSTON_CFLAGS}
        -Wall -Wextra -Werror -Wno-unused-parameter -g -pedantic
        -Wsign-promo -Winit-self
        -Wmissing-include-dirs -Wframe-larger-than=8192 -Wcast-align
        -Wlogical-op -Wmissing-format-attribute
        -Woverloaded-virtual -Wstrict-null-sentinel -std=c++0x 
        #-Wfloat-equal -Wno-error=float-equal
       )
endif()

if (MSVC)
    set(CUSTOM_CFLAGS ${CUSTOM_CFLAGS}
        /wd4244 # narrowing conversion from 'x' to 'y', possible loss of data
        /wd4305 # truncation from 'x' to 'y'
        /wd4351 # "warning: daring to follow C++ initialization standard"
        /wd4355 # 'this' used in member initialization
        /wd4800 # forcing value to bool 'true' or 'false'
    )
endif()

add_subdirectory(${laroot_SOURCE_DIR}/extern)
add_subdirectory(${laroot_SOURCE_DIR}/la)

