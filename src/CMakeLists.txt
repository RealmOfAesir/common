cmake_minimum_required(VERSION 3.5.0 FATAL_ERROR)
project(RealmOfAesirCommon CXX)

file(GLOB PROJECT_SOURCES ${PROJECT_SOURCE_DIR}/*.cpp)
add_library(RealmOfAesirCommon ${PROJECT_SOURCES})

include_directories("${PROJECT_SOURCE_DIR}")

include_directories("${CMAKE_SOURCE_DIR}/external")
include_directories("${CMAKE_SOURCE_DIR}/external/librdkafka/src-cpp")
include_directories("${CMAKE_SOURCE_DIR}/external/cereal/include")
include_directories("${CMAKE_SOURCE_DIR}/external/easyloggingpp/src")
include_directories("${CMAKE_SOURCE_DIR}/external/json/src")
include_directories("${CMAKE_SOURCE_DIR}/external/di/include")

find_library(LIBRDKAFKA_LIBRARY rdkafka HINTS ${CMAKE_SOURCE_DIR}/external/librdkafka/src)
find_library(LIBRDKAFKAPP_LIBRARY rdkafka++ HINTS ${CMAKE_SOURCE_DIR}/external/librdkafka/src-cpp)

target_link_libraries(RealmOfAesirCommon PUBLIC ${LIBRDKAFKA_LIBRARY})
target_link_libraries(RealmOfAesirCommon PUBLIC ${LIBRDKAFKAPP_LIBRARY})