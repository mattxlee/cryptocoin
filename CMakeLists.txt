cmake_minimum_required(VERSION 2.7)

project(blockchain)

find_package(OpenSSL REQUIRED)

set(CMAKE_CXX_FLAGS "-std=c++11")

include_directories(${OPENSSL_INCLUDE_DIR})

file(GLOB src "./src/*.cc")

add_library(blockchain ${src})

target_link_libraries(blockchain ${OPENSSL_LIBRARIES})

