cmake_minimum_required(VERSION 3.5.0 FATAL_ERROR)
project(Test CXX)

file(GLOB PROJECT_SOURCES ${PROJECT_SOURCE_DIR}/*.cpp)
add_executable(Test ${PROJECT_SOURCES} main.cpp)

add_test(NAME MyTest COMMAND Test)


include_directories("${CMAKE_SOURCE_DIR}/src")
include_directories("${CMAKE_SOURCE_DIR}/external")
include_directories("${CMAKE_SOURCE_DIR}/external/librdkafka/src-cpp")
include_directories("${CMAKE_SOURCE_DIR}/external/cereal/include")
include_directories("${CMAKE_SOURCE_DIR}/external/easyloggingpp/src")
include_directories("${CMAKE_SOURCE_DIR}/external/json/src")
include_directories("${CMAKE_SOURCE_DIR}/external/di/include")
include_directories("${CMAKE_SOURCE_DIR}/external/doctest/doctest")

target_link_libraries(Test PUBLIC RealmOfAesirCommon)