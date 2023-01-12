cmake_minimum_required(VERSION 3.23)
project(FluidSim)

set(CMAKE_CXX_FLAGS_RELEASE  "-O3")

if(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 ")
endif(UNIX)

if (WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -lmingw32")
endif(WIN32)

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

add_compile_definitions(STB_IMAGE_IMPLEMENTATION)

add_subdirectory("include")
add_subdirectory("src")
