cmake_minimum_required(VERSION 3.7)

project(test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)


# set(GCC_COVERAGE_COMPILE_FLAGS "-g -O0 -coverage -fprofile-arcs -ftest-coverage")
# set(GCC_COVERAGE_LINK_FLAGS    "-coverage -lgcov")
# set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )
# set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}" )

# Release MinSizeRel
set(CMAKE_BUILD_TYPE Debug)

file( GLOB SRCS *.cpp *.h )

add_executable(test ${SRCS})
target_link_libraries(test  ${CMAKE_THREAD_LIBS_INIT})
