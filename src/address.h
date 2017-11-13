#ifndef __ADDRESS_H__
#define __ADDRESS_H__

#include <cstdlib>
#include <stdlib.h>

#include <string>
#include <vector>

namespace coin {

class Address {
 public:
  /**
   * Convert a public key to address.
   *
   * @param pub_key Public key.
   *
   * @return New generated address object.
   */
  static Address FromPublicKey(const std::vector<uint8_t> &pub_key);

  /// Convert address object to string
  std::string ToString() const { return addr_str_; }

 private:
  Address() {}

 private:
  std::string addr_str_;
};

}  // namespace coin

#endif
