#include <sstream>
#include <string>

#include "data_value.h"
#include "gtest/gtest.h"

template <typename T>
std::tuple<T, bool> StreamReadWriteValCompare() {
  srand(time(NULL));
  size_t max = std::numeric_limits<T>::max();
  T value = (static_cast<double>(rand()) / RAND_MAX) * max;
  std::stringstream ss;
  coin::data::MakeValue(value).WriteToStream(ss);
  coin::data::Value<T> value_obj;
  value_obj.ReadFromStream(ss);
  return std::make_tuple(value, value == value_obj.value);
}

TEST(DataValue, Int8) {
  uint8_t value;
  bool test_result;
  std::tie(value, test_result) = StreamReadWriteValCompare<uint8_t>();
  EXPECT_TRUE(true) << "Random value for " << sizeof(uint8_t) * 8
                    << " bits integer is " << std::to_string(value);
}

TEST(DataValue, Int16) {
  uint16_t value;
  bool test_result;
  std::tie(value, test_result) = StreamReadWriteValCompare<uint16_t>();
  EXPECT_TRUE(true) << "Random value for " << sizeof(uint16_t) * 8
                    << " bits integer is " << std::to_string(value);
}

TEST(DataValue, Int32) {
  uint32_t value;
  bool test_result;
  std::tie(value, test_result) = StreamReadWriteValCompare<uint32_t>();
  EXPECT_TRUE(true) << "Random value for " << sizeof(uint32_t) * 8
                    << " bits integer is " << std::to_string(value);
}

TEST(DataValue, Int64) {
  uint64_t value;
  bool test_result;
  std::tie(value, test_result) = StreamReadWriteValCompare<uint64_t>();
  EXPECT_TRUE(true) << "Random value for " << sizeof(uint64_t) * 8
                    << " bits integer is " << std::to_string(value);
}

TEST(DataValue, String) {
  const std::string TEST_STRING = "Hello World!";
  auto value_obj = coin::data::MakeValue(TEST_STRING);
  std::stringstream ss;
  value_obj.WriteToStream(ss);
  value_obj.value.clear();
  value_obj.ReadFromStream(ss);
  EXPECT_EQ(value_obj.value, TEST_STRING);
}
