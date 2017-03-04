cmake_minimum_required(VERSION 3.5.0 FATAL_ERROR)
project(RealmOfAesirCommon CXX)

file(GLOB_RECURSE MSG_SRCS ${PROJECT_SOURCE_DIR}/messages/*.cpp)
file(GLOB_RECURSE ADMIN_MSG_SRCS ${PROJECT_SOURCE_DIR}/admin_messages/*.cpp)
file(GLOB PROJECT_SOURCES ${PROJECT_SOURCE_DIR}/*.cpp ${MSG_SRCS} ${ADMIN_MSG_SRCS})
add_library(RealmOfAesirCommon ${PROJECT_SOURCES})

include_directories("${PROJECT_SOURCE_DIR}")

include_directories("${CMAKE_SOURCE_DIR}/external")
include_directories("${CMAKE_SOURCE_DIR}/external/librdkafka/src-cpp")
include_directories("${CMAKE_SOURCE_DIR}/external/cereal/include")
include_directories("${CMAKE_SOURCE_DIR}/external/easyloggingpp/src")
include_directories("${CMAKE_SOURCE_DIR}/external/json/src")
include_directories("${CMAKE_SOURCE_DIR}/external/di/include")

target_link_libraries(RealmOfAesirCommon PUBLIC ${CMAKE_SOURCE_DIR}/external/librdkafka/src/librdkafka.a)
target_link_libraries(RealmOfAesirCommon PUBLIC ${CMAKE_SOURCE_DIR}/external/librdkafka/src-cpp/librdkafka++.a)