cmake_minimum_required(VERSION 2.8)
project(gnuplot++)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
     message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

set(GNUPLOTPP_MAJOR 0)
set(GNUPLOTPP_MINOR 1)
set(GNUPLOTPP_PATCH 0)

set(GNUPLOTPP_VERSION "${GNUPLOTPP_MAJOR}.${GNUPLOTPP_MINOR}.${GNUPLOTPP_PATCH}")

message(STATUS "Building GnuplotPP ${GNUPLOTPP_VERSION}")

# set cmake properties
include(cmake/FindGnuplot.cmake)

# compile sources
add_subdirectory(src)

#set includes
include_directories(src)
include_directories(.)

# compile tools and examples
add_subdirectory(tools)
add_subdirectory(examples)