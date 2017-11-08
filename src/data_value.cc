#include "data_value.h"

#include <arpa/inet.h>

namespace coin {
namespace data {

namespace utils {

std::vector<uint8_t> ToData(const std::string &str) {
  std::vector<uint8_t> data(str.size() + sizeof(uint32_t));
  uint32_t size = htonl(str.size());
  memcpy(data.data(), &size, sizeof(size));
  memcpy(data.data() + sizeof(size), str.c_str(), str.size());
  return data;
}

std::vector<uint8_t> ToData(const uint16_t value) {
  std::vector<uint8_t> data(sizeof(value));
  uint16_t value_n = htons(value);
  memcpy(data.data(), &value_n, sizeof(value_n));
  return data;
}

std::vector<uint8_t> ToData(const uint32_t value) {
  std::vector<uint8_t> data(sizeof(value));
  uint32_t value_n = htonl(value);
  memcpy(data.data(), &value_n, sizeof(value_n));
  return data;
}

std::vector<uint8_t> ToData(const uint64_t value) {
  std::vector<uint8_t> data(sizeof(value));
  uint64_t value_n = htonll(value);
  memcpy(data.data(), &value_n, sizeof(value_n));
  return data;
}

size_t FromData(DataIterator begin, DataIterator end, std::string &out) {
  assert((size_t)std::distance(begin, end) > sizeof(uint32_t));
  uint32_t size;
  memcpy(&size, &(*begin), sizeof(size));
  size = ntohl(size);
  assert((size_t)std::distance(begin, end) >= sizeof(uint32_t) + size);
  char *buf = new char[size + 1];
  memcpy(buf, &(*begin) + sizeof(uint32_t), size);
  buf[size] = '\0';
  out = buf;
  delete[] buf;
  return out.size() + sizeof(size);
}

size_t FromData(DataIterator begin, DataIterator end, uint16_t &out) {
  assert((size_t)std::distance(begin, end) >= sizeof(out));
  memcpy(&out, &(*begin), sizeof(out));
  out = ntohs(out);
  return sizeof(out);
}

size_t FromData(DataIterator begin, DataIterator end, uint32_t &out) {
  assert((size_t)std::distance(begin, end) >= sizeof(out));
  memcpy(&out, &(*begin), sizeof(out));
  out = ntohl(out);
  return sizeof(out);
}

size_t FromData(DataIterator begin, DataIterator end, uint64_t &out) {
  assert((size_t)std::distance(begin, end) >= sizeof(out));
  memcpy(&out, &(*begin), sizeof(out));
  out = ntohll(out);
  return sizeof(out);
}

}  // namespace utils

}  // namespace data
}  // namespace coin
