cmake_minimum_required(VERSION 3.5.0 FATAL_ERROR)
project(RealmOfAesirCommon)
#set(CMAKE_CXX_STANDARD 17) #not supported yet :(
#set(CMAKE CXX STANDARD REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Debug CACHE STRING
            "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
            FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# only support linux/g++ for now
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DELPP_THREAD_SAFE -Wall -Wno-long-long -Wno-unused-variable -pedantic -std=c++17 ")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -ggdb -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g -O2")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os")

add_subdirectory(src)

enable_testing()
add_subdirectory(test)
