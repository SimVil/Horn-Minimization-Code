# -----------------------------------------------------------------------------
# File:    CMAKE script
# Project: Algorithms
# Author:  Simon Vilmin
# Date:    2018
#
# Aims to setup Algorithms project structure.
# -----------------------------------------------------------------------------

# ==== General settings ==== #
cmake_minimum_required(VERSION 3.9)
project(Algorithms)
set(CMAKE_CXX_STANDARD 11)


# ==== Files to handle ==== #
# CanonicalBasis/ contains all algorithms .h and .cpp
include_directories(CanonicalBasis)
file(GLOB ALGO_HEADER "CanonicalBasis/*.h")
file(GLOB ALGO_CODE "CanonicalBasis/*.cpp")

# Tests/ contains unit testing interfaces and mains.
include_directories(Tests)
file(GLOB TEST_INTERFACE "Tests/Tester/Tester.cpp Tests/Tester/Tester.h")
file(GLOB TEST_MAIER "Tests/Tester/TestMaier.cpp Tests/Tester/TestMaier.h")
file(GLOB TEST_BERCZI "Tests/Tester/TestBerczi.cpp Tests/Tester/TestBerczi.h")


# ==== Boost settings ==== #
set(BOOST_ROOT "${PROJECT_SOURCE_DIR}/boost")
set(BOOST_LIBRARYDIR "${BOOST_ROOT}/lib")
set(Boost_USE_STATIC_LIBS ON)

message(WARNING "Pouet ${PROJECT_SOURCE_DIR}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

find_package(Boost REQUIRED)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
endif()


# ==== Targets and executables ==== #

add_executable(Algorithms
        main.cpp
        ${ALGO_HEADER}
        ${ALGO_CODE}
        )
target_link_libraries(Algorithms)

add_executable(TestMaier
        catch.h
        Tests/Test_Maier_Unit.cpp
        ${TEST_INTERFACE}
		${TEST_MAIER}
        ${ALGO_HEADER}
        ${ALGO_CODE}
        )

add_executable(TestBerczi
        catch.h
		Tests/Test_Berczi_Unit.cpp
        ${TEST_INTERFACE}
		${TEST_BERCZI}
        ${ALGO_HEADER}
        ${ALGO_CODE}
        )



