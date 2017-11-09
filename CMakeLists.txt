cmake_minimum_required(VERSION 3.0)

project(cryptocoin)

find_package(OpenSSL REQUIRED)

find_library(SECP256K1 secp256k1)

set(CMAKE_CXX_FLAGS "-std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "-g")

include_directories(
  ${OPENSSL_INCLUDE_DIR}
  "./src"
  )

file(GLOB CRYPTOCOIN_SRC "./src/*.cc")

# ==== GOOGLE TEST ====
include_directories(
  "./googletest/googletest/"
  "./googletest/googletest/include"
  )
file(GLOB GOOGLE_TEST_SRC "./googletest/googletest/src/gtest-all.cc")
file(GLOB GOOGLE_TEST_MAIN "./googletest/googletest/src/gtest_main.cc")
# ==== GOOGLE TEST ====

# ==== Data Value Test ====
set(DATA_VALUE_TEST_SRC "test/data_value_test.cc")
# ==== Data Value Test ====

add_executable(data_value_test
  ${GOOGLE_TEST_SRC}
  ${GOOGLE_TEST_MAIN}
  ${DATA_VALUE_TEST_SRC}
  )

