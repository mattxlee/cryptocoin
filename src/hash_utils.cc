#include "hash_utils.h"

#include <arpa/inet.h>

#include <cassert>
#include <sstream>
#include <string>

namespace coin {

Hash256::Hash256() { SHA256_Init(&ctx_); }

Hash256::~Hash256() { Final(); }

void Hash256::Calculate(const uint8_t *p, size_t size) {
  assert(p != nullptr && size > 0);
  assert(SHA256_Update(&ctx_, p, size) == 1);
}

void Hash256::Final() {
  assert(!finished_);
  assert(SHA256_Final(md_, &ctx_) == 1);
  finished_ = true;
}

std::string HashToStr(const DataValue &hash, int num_of_digits) {
  std::stringstream ss;
  char digits[3];
  int n = 0;
  auto it = std::begin(hash);
  while (n < num_of_digits && it < std::end(hash)) {
    sprintf(digits, "%02x", *it);
    ss << digits;
    // next
    ++n;
    ++it;
  }
  return ss.str();
}

DataValue ToDataValue(const int &value) {
  int value_n = htonl(value);
  DataValue data(sizeof(value_n));
  memcpy(data.data(), &value_n, sizeof(value_n));
  return data;
}

DataValue ToDataValue(const short &value) {
  short value_n = htons(value);
  DataValue data(sizeof(value_n));
  memcpy(data.data(), &value_n, sizeof(value_n));
  return data;
}

DataValue ToDataValue(const uint64_t &value) {
  uint64_t value_n = htonll(value);
  DataValue data(sizeof(value_n));
  memcpy(data.data(), &value_n, sizeof(value_n));
  return data;
}

}  // namespace coin
