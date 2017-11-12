#ifndef __BLOCK_H__
#define __BLOCK_H__

#include <string>

#include "big_num.h"
#include "data_value.h"
#include "transaction.h"

namespace coin {
namespace blk {

/**
 * Block structure
 */
class Block {
 public:
  template <typename Stream>
  void Serialize(Stream &s) {}

  template <typename Stream>
  void Unserialize(Stream &s) {}

 private:
  // Block base info.
  bn::HashNum block_hash_;
  int version_ = 1;
  time_t timestamp_ = 0;
  uint32_t height_ = -1;
  // Below values contain in the calculation of block hash.
  bn::HashNum prev_hash_;
  bn::HashNum merkle_root_hash_;
  uint32_t nonce_ = 0;
  bn::HashNum diff_;
  std::vector<Transaction> trans_;
};

}  // namespace blk
}  // namespace coin

#endif
