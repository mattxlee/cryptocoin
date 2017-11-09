#include <sstream>
#include <string>

#include "gtest/gtest.h"

#include "data_value.h"
#include "transaction.h"

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

TEST(HostAndNet, Int16Swap) {
  const uint16_t TEST_VALUE_HOST = 0x1020;
  const uint16_t TEST_VALUE_NET = 0x2010;
  uint16_t test_value_net = coin::data::utils::HostToNet(TEST_VALUE_HOST);
  EXPECT_EQ(test_value_net, TEST_VALUE_NET);
}

TEST(HostAndNet, Int32Swap) {
  const uint32_t TEST_VALUE_HOST = 0x10203040;
  const uint32_t TEST_VALUE_NET = 0x40302010;
  uint32_t test_value_net = coin::data::utils::HostToNet(TEST_VALUE_HOST);
  EXPECT_EQ(test_value_net, TEST_VALUE_NET);
}

TEST(HostAndNet, Int64Swap) {
  const uint64_t TEST_VALUE_HOST = 0x1020304050607080;
  const uint64_t TEST_VALUE_NET = 0x8070605040302010;
  uint64_t test_value_net = coin::data::utils::HostToNet(TEST_VALUE_HOST);
  EXPECT_EQ(test_value_net, TEST_VALUE_NET);
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
  EXPECT_EQ(value_obj.value, TEST_STRING) << "String value: " << TEST_STRING;
}

/// Randomized data.
std::vector<uint8_t> g_random_data;
const uint32_t g_random_data_size = 1024 * 1024 * 2;

/// Data-trunk related.
const uint32_t g_bytes_each_trunk = 102400;
struct Trunk {
  std::vector<uint8_t> data;
};
std::vector<Trunk> vec_trunk;

/// Make randomized data.
std::vector<uint8_t> MakeRandomData(int num_of_bytes) {
  std::vector<uint8_t> result(num_of_bytes);
  srand(time(NULL));
  for (int i = 0; i < num_of_bytes; ++i) {
    result[i] = rand() % 256;
  }
  return result;
}

TEST(MerkleTree, BuildRandomData) {
  g_random_data = MakeRandomData(g_random_data_size);
  EXPECT_EQ(g_random_data.size(), g_random_data_size);
}

TEST(MerkleTree, BuildNodeTrunkList) {
  auto begin = g_random_data.begin(), end = g_random_data.end();
  while (begin != end) {
    size_t copy_size = g_bytes_each_trunk;
    size_t left_size = std::distance(begin, end);
    if (left_size < g_bytes_each_trunk) {
      copy_size = left_size;
    }
    Trunk trunk;
    trunk.data.resize(copy_size);
    std::memcpy(trunk.data.data(), &(*begin), copy_size);
    vec_trunk.push_back(std::move(trunk));
    begin += copy_size;
  }
  EXPECT_EQ(vec_trunk.size(), g_random_data_size / g_bytes_each_trunk + 1);
}
