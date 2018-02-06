cmake_minimum_required(VERSION 3.9)
project(Algorithms)

set(CMAKE_CXX_STANDARD 11)
set(BOOST_ROOT "C:/Program Files (x86)/boost/boost_1_66_0")

find_package(Boost)
IF (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
endif()

include_directories(CanonicalBasis)
include_directories(HornMinimization)
file(GLOB HEADER "CanonicalBasis/*.h")
file(GLOB CODE "CanonicalBasis/*.cpp")
add_executable(Algorithms ${HEADER} ${CODE} main.cpp)

add_executable(TestLinClosure
        Tests/Tests_LinClosure.cpp
        ${HEADER}
        catch.h )