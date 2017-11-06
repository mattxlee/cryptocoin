#include "hash_utils.h"

namespace coin {

Hash256::Hash256() { SHA256_Init(&ctx_); }

Hash256::~Hash256() { Final(); }

bool Hash256::Calculate(const uint8_t *p, size_t size) {
  int ret = SHA256_Update(&ctx_, p, size);
  return ret == 1;
}

bool Hash256::Final() {
  if (finished_) return false;
  int ret = SHA256_Final(md_, &ctx_);
  finished_ = true;
  return ret == 1;
}

}  // namespace coin
