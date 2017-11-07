#ifndef __HASH_UTILS_H__
#define __HASH_UTILS_H__

#include <vector>

#include <openssl/sha.h>

namespace coin {

/// Basic data-value store.
typedef std::vector<uint8_t> DataValue;

/// Hash 256 algorithm.
class Hash256 {
 public:
  Hash256();
  ~Hash256();

  bool Calculate(const uint8_t *p, size_t size);
  bool Final();

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
  HashBuilder &operator<<(const DataValue &data) {
    if (algo_.is_finished() || !algo_.Calculate(data.data(), data.size())) {
      // TODO Failed!
    }
    return *this;
  }

  DataValue FinalValue() {
    if (!algo_.Final()) {
      // TODO Failed!
    }
    DataValue data(algo_.get_md_size());
    memcpy(data.data(), algo_.get_md(), data.size());
    return data;
  }

 private:
  HashAlgo algo_;
};

typedef HashBuilder<Hash256> Hash256Builder;

/// Convert DataValue to string.
std::string HashToStr(const DataValue &hash, int num_of_digits = 4);

/// Convert value to DataValue.
DataValue ToDataValue(const int &value);

/// Convert value to DataValue.
DataValue ToDataValue(const short &value);

}  // namespace coin

#endif
