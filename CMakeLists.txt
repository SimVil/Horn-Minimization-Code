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
add_executable(Algorithms ${HEADER} ${CODE}
        main.cpp)

add_executable(TestMaier
        catch.h
        Tests/Test_Maier_Unit.cpp
        Tests/Testers/Tester.cpp
        Tests/Testers/Tester.h
        Tests/Testers/MaierTester.cpp
        Tests/Testers/MaierTester.h
        ${HEADER}
        ${CODE}
        )

add_executable(TestBerczi
        Tests/Test_Berczi_Unit.cpp
        ${HEADER}
        ${CODE}
        catch.h)