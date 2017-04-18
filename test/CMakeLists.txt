cmake_minimum_required(VERSION 3.5.0 FATAL_ERROR)
project(Test CXX)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB PROJECT_SOURCES ${PROJECT_SOURCE_DIR}/*.cpp ${PROJECT_SOURCE_DIR}/test_helpers/*.cpp)
add_executable(Test ${PROJECT_SOURCES} main.cpp)

add_test(NAME MyTest COMMAND Test)


include_directories("${CMAKE_SOURCE_DIR}/src")
include_directories("${CMAKE_SOURCE_DIR}/external")
include_directories("${CMAKE_SOURCE_DIR}/external/librdkafka/src-cpp")
include_directories("${CMAKE_SOURCE_DIR}/external/cereal/include")
include_directories("${CMAKE_SOURCE_DIR}/external/easyloggingpp/src")
include_directories("${CMAKE_SOURCE_DIR}/external/json/src")
include_directories("${CMAKE_SOURCE_DIR}/external/di/include")
include_directories("${CMAKE_SOURCE_DIR}/external/Catch/include")

find_library(LIBRDKAFKA_LIBRARY rdkafka HINTS ${CMAKE_SOURCE_DIR}/external/librdkafka/src)
find_library(LIBRDKAFKAPP_LIBRARY rdkafka++ HINTS ${CMAKE_SOURCE_DIR}/external/librdkafka/src-cpp)

target_link_libraries(Test PUBLIC RealmOfAesirCommon)
target_link_libraries(Test PUBLIC ${LIBRDKAFKAPP_LIBRARY})
target_link_libraries(Test PUBLIC ${LIBRDKAFKA_LIBRARY})

find_package( ZLIB REQUIRED )
include_directories( ${ZLIB_INCLUDE_DIRS} )
target_link_libraries(Test PUBLIC ${ZLIB_LIBRARIES} )

find_package (Threads)
target_link_libraries (Test PUBLIC ${CMAKE_THREAD_LIBS_INIT})

find_package(OpenSSL REQUIRED)
target_link_libraries (Test PUBLIC ${OPENSSL_LIBRARIES})