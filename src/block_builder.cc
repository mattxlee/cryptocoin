#include "block_builder.h"

#define ZERO_HASH256 \
  "0000000000000000000000000000000000000000000000000000000000000000"
//  1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2

namespace coin {
namespace blk {

Block BlockBuilder::BuildGenesisBlock() {
  Block block;
  block.set_timestamp(time(NULL));
  block.set_height(0);
  block.set_block_hash(ZERO_HASH256);
  block.set_prev_hash(ZERO_HASH256);
  block.set_merkle_root_hash(ZERO_HASH256);
  block.set_difficult(ZERO_HASH256);
  return block;
}

}  // namespace blk
}  // namespace coin
