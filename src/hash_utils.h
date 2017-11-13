#ifndef __HASH_UTILS_H__
#define __HASH_UTILS_H__

#include <vector>

#include <openssl/ripemd.h>
#include <openssl/sha.h>

#include "data_value.h"

namespace coin {

/// RIPEMD160 (Hash) algorithm.
class Hash160 {
 public:
  Hash160();
  ~Hash160();

  void Calculate(const uint8_t *p, size_t size);
  void Final();

  const uint8_t *get_md() const { return md_; }
  size_t get_md_size() const { return RIPEMD160_DIGEST_LENGTH; }

  bool is_finished() const { return finished_; }

 private:
  uint8_t md_[RIPEMD160_DIGEST_LENGTH];
  RIPEMD160_CTX ctx_;
  bool finished_ = false;
};

/// Hash 256 algorithm.
class Hash256 {
 public:
  Hash256();
  ~Hash256();

  void Calculate(const uint8_t *p, size_t size);
  void Final();

  const uint8_t *get_md() const { return md_; }
  size_t get_md_size() const { return SHA256_DIGEST_LENGTH; }

  bool is_finished() const { return finished_; }

 private:
  uint8_t md_[SHA256_DIGEST_LENGTH];
  SHA256_CTX ctx_;
  bool finished_ = false;
};

/// Hash value builder.
template <typename HashAlgo>
class HashBuilder {
 public:
  template <typename DataValue>
  HashBuilder &operator<<(const DataValue &value) {
    assert(!algo_.is_finished());
    auto data = value.MakeStreamData();
    algo_.Calculate(data.data(), data.size());
    return *this;
  }

  data::Buffer FinalValue() {
    algo_.Final();
    data::Buffer buffer;
    buffer.CopyFrom(algo_.get_md(), algo_.get_md_size());
    return buffer;
  }

  static std::vector<uint8_t> CalculateHash(const std::vector<uint8_t> &data) {
    return CalculateHash(data.data(), data.size());
  }

  static std::vector<uint8_t> CalculateHash(const uint8_t *data, size_t size) {
    HashAlgo algo;
    algo.Calculate(data, size);
    algo.Final();
    std::vector<uint8_t> result(algo.get_md_size());
    memcpy(result.data(), algo.get_md(), algo.get_md_size());
    return result;
  }

 private:
  HashAlgo algo_;
};

typedef HashBuilder<Hash160> Hash160Builder;
typedef HashBuilder<Hash256> Hash256Builder;

/// Convert DataValue to string.
std::string HashToStr(const data::Buffer &hash, int num_of_digits = 4);

}  // namespace coin

#endif
