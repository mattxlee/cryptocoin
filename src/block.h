#ifndef __BLOCK_H__
#define __BLOCK_H__

#include <string>

#include "hash_utils.h"

namespace coin {

class Block {
 public:
  template <typename Stream>
  size_t Serialize(Stream &s) {}

  template <typename Stream>
  void Unserialize(const Stream &s) {}

 private:
  // Block base info.
  DataValue block_hash_;
  int version_ = 0x01000000;
  time_t timestamp_;
  // Below values contain in the calculation of block hash.
  DataValue prev_hash_;
  DataValue merkl_root_hash_;
  DataValue nonce_;
};

}  // namespace coin

#endif
