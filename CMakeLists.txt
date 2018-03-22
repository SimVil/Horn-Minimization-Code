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
# we must change, using file for gathering is deprecated by the doc
# try to use list instead
include_directories(CanonicalBasis)
file(GLOB ALGO_HEADER "CanonicalBasis/*.h")
file(GLOB ALGO_CODE "CanonicalBasis/*.cpp")

# Tests/ contains unit testing interfaces and mains.
include_directories(Tests)
list(APPEND TEST_INTERFACE Tests/Testers/Tester.cpp Tests/Testers/Tester.h)
list(APPEND TEST_MAIER Tests/Testers/MaierTester.cpp Tests/Testers/MaierTester.h)
list(APPEND TEST_BERCZI Tests/Testers/BercziTester.cpp Tests/Testers/BercziTester.h)
list(APPEND IMP_TOOLS Tests/Tools/ImplicationTools.h Tests/Tools/ImplicationTools.cpp)


# ==== Boost settings ==== #
set(BOOST_ROOT "${PROJECT_SOURCE_DIR}/boost")
set(BOOST_LIBRARYDIR "${BOOST_ROOT}/lib")
set(Boost_USE_STATIC_LIBS ON)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

find_package(Boost REQUIRED)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
endif()


# ==== Targets and executables ==== #

add_executable(Algorithms
        catch.h
        main.cpp
        ${IMP_TOOLS}
        ${ALGO_HEADER}
        ${ALGO_CODE})

target_link_libraries(Algorithms
        ${BOOST_LIBRARYDIR}/libboost_timer-mgw63-mt-s-x32-1_66.a
        ${BOOST_LIBRARYDIR}/libboost_system-mgw63-mt-s-x32-1_66.a
        ${BOOST_LIBRARYDIR}/libboost_chrono-mgw63-mt-s-x32-1_66.a)

#target_link_libraries(Algorithms
#        ${BOOST_LIBRARYDIR}/libboost_timer-mgw63-mt-s-x32-1_66.a
#        ${BOOST_LIBRARYDIR}/libboost_system-mgw63-mt-s-x32-1_66.a
#        ${BOOST_LIBRARYDIR}/libboost_chrono-mgw63-mt-s-x32-1_66.a)

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



