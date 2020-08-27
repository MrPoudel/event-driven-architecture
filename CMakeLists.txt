cmake_minimum_required(VERSION 3.7)

project(test)

set(CMAKE_CXX_STANDARD 17)
add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)


# set(GCC_COVERAGE_COMPILE_FLAGS "-g -O0 -coverage -fprofile-arcs -ftest-coverage")
# set(GCC_COVERAGE_LINK_FLAGS    "-coverage -lgcov")
# set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )
# set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}" )

# Release MinSizeRel
set(CMAKE_BUILD_TYPE Debug)

set(SRCS Fault.cpp Fault.h  Thread.cpp Thread.hpp ClassA.hpp ClassB.hpp Observer.hpp)#AnyCallable.hpp)ClassA.hpp 

add_library(lib ${SRCS})
target_link_libraries(lib  ${CMAKE_THREAD_LIBS_INIT})

add_executable(test main.cpp)
target_link_libraries(test lib)
