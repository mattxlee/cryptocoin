cmake_minimum_required(VERSION 3.0)

project(cryptocoin)

find_package(OpenSSL REQUIRED)

find_library(SECP256K1 secp256k1)

set(CMAKE_CXX_FLAGS "-std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "-g")

include_directories(${OPENSSL_INCLUDE_DIR})

file(GLOB src "./src/*.cc")

add_library(cryptocoin SHARED ${src})

target_link_libraries(cryptocoin ${OPENSSL_LIBRARIES} ${SECP256K1})

