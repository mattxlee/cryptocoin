#ifndef __BIG_NUM_H__
#define __BIG_NUM_H__

#include <cinttypes>
#include <regex>

#include "data_value.h"

namespace coin {
namespace bn {

template <int N>
class BigNum {
 public:
  BigNum() {}
  explicit BigNum(const uint8_t *value) { memcpy(digits_, value, N); }

  static BigNum<N> FromString(const std::string &str) {
    std::regex r("^0x([0-9a-f]+)$");
    std::smatch m;
    assert(std::regex_match(str, m, r));

    std::string digits_str = m[1].str();
    assert(digits_str.size() == N * 2);

    uint8_t val[N];
    for (int i = 0; i < digits_str.size(); i += 2) {
      char digit[3];
      digit[0] = digits_str[i];
      digit[1] = digits_str[i + 1];
      digit[2] = '\0';
      int val_scan;
      sscanf(digit, "%x", &val_scan);
      val[i / 2] = val_scan % 256;
    }

    BigNum<N> num(val);
    return num;
  }

  BigNum<N> &operator=(const BigNum<N> &rhs) {
    if (this != &rhs) {
      memcpy(digits_, rhs.digits_, N * sizeof(uint8_t));
    }
    return *this;
  }

  bool operator==(const BigNum &another) const {
    for (int i = 0; i < N; ++i) {
      if (digits_[i] != another.digits_[i]) return false;
    }
    return true;
  }

  bool operator!=(const BigNum &another) const { return !(*this == another); }

  bool operator<(const BigNum &another) const {
    for (int i = 0; i < N; ++i) {
      if (digits_[i] != another.digits_[i]) {
        return digits_[i] < another.digits_[i];
      }
    }
    return false;
  }

  bool operator>(const BigNum &another) const {
    return !(*this < another) && *this != another;
  }

  const uint8_t *get_data() const { return digits_; }
  uint8_t *get_data() { return digits_; }

 private:
  uint8_t digits_[N];
};

typedef BigNum<32> HashNum;

}  // namespace bn

namespace data {

template <int N>
class Value<bn::BigNum<N>> {
 public:
  Value() {}
  Value(const bn::BigNum<N> &another) : num_(another) {}

  void set_num(const bn::BigNum<N> &num) { num_ = num; }

  const bn::BigNum<N> &get_num() const { return num_; }

  template <typename Stream>
  void WriteToStream(Stream &s) const {
    s.write((const char *)num_.get_data(), sizeof(uint8_t) * N);
  }

  template <typename Stream>
  void ReadFromStream(Stream &s) const {
    s.read((char *)num_.get_data(), sizeof(uint8_t) * N);
  }

  std::vector<uint8_t> MakeStreamData() const {
    std::vector<uint8_t> data(N);
    memcpy(data.data(), num_.digits_, N * sizeof(uint8_t));
    return data;
  }

 private:
  bn::BigNum<N> num_;
};

}  // namespace data
}  // namespace coin

#endif
