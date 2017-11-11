#ifndef __BLOCK_H__
#define __BLOCK_H__

#include <string>

#include "data_value.h"
#include "transaction.h"

namespace coin {

class Block {
 public:
  template <typename Stream>
  void Serialize(Stream &s) {}

  template <typename Stream>
  void Unserialize(Stream &s) {}

 private:
  // Block base info.
  data::Buffer block_hash_;
  int version_ = 1;
  time_t timestamp_;
  uint32_t height_;
  // Below values contain in the calculation of block hash.
  data::Buffer prev_hash_;
  data::Buffer merkl_root_hash_;
  data::Buffer nonce_;
  std::vector<Transaction> trans_;
};

}  // namespace coin

#endif
