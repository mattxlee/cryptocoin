#ifndef __HASH_UTILS_H__
#define __HASH_UTILS_H__

#include <vector>

#include <openssl/sha.h>

#include "data_value.h"

namespace coin {

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
    value.WithData([this](data::ConstDataIterator begin, data::ConstDataIterator end) {
      algo_.Calculate(&(*begin), std::distance(begin, end));
    });
    return *this;
  }

  data::Buffer FinalValue() {
    algo_.Final();
    data::Buffer buffer;
    buffer.CopyFrom(algo_.get_md(), algo_.get_md_size());
    return buffer;
  }

 private:
  HashAlgo algo_;
};

typedef HashBuilder<Hash256> Hash256Builder;

/// Convert DataValue to string.
std::string HashToStr(const data::Buffer &hash, int num_of_digits = 4);

}  // namespace coin

#endif
