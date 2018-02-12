cmake_minimum_required(VERSION 3.9)
project(Algorithms)

set(CMAKE_CXX_STANDARD 11)
# set(BOOST_ROOT "/home/simon/Documents/boost_1_66_0")
set(BOOST_ROOT "C:/Program Files/boost/boost_1_66_0")

SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS}" )
SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS}" )



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
        catch.h
        Tests/Test_Berczi_Unit.cpp
        Tests/Testers/Tester.cpp
        Tests/Testers/Tester.h
        Tests/Testers/BercziTester.cpp
        Tests/Testers/BercziTester.h
        ${HEADER}
        ${CODE}
        )


# target_link_libraries(Algorithms profiler boost_timer)
find_library(BOOST_TIMER libboost_timer-mgw63-mt-sd-x32-1_66 "C:/Program Files/boost/boost_1_66_0/stage/lib")
find_library(BOOST_SYSTEM libboost_system-mgw63-mt-sd-x32-1_66 "C:/Program Files/boost/boost_1_66_0/stage/lib")
find_library(BOOST_CHRONO libboost_chrono-mgw63-mt-sd-x32-1_66 "C:/Program Files/boost/boost_1_66_0/stage/lib")
target_link_libraries(Algorithms ${BOOST_TIMER} ${BOOST_SYSTEM} ${BOOST_CHRONO})
