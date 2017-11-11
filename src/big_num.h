#ifndef __BIG_NUM_H__
#define __BIG_NUM_H__

#include <cinttypes>

namespace coin {
namespace bn {

template <int N>
class BigNum {
 public:
  BigNum() {}
  explicit BigNum(uint8_t *value, int len = N) { memcpy(digits_, value, len); }

  bool operator==(const BigNum &another) const {
    for (int i = 0; i < N; ++i) {
      if (digits_[i] != another.digits_[i]) return false;
    }
    return true;
  }

  bool operator!=(const BigNum &another) const {
    return !(*this == another);
  }

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

 private:
  uint8_t digits_[N];
};

}  // namespace bn
}  // namespace coin

#endif
