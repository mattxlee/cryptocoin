#ifndef __BLOCK_BUILDER_H__
#define __BLOCK_BUILDER_H__

#include "block.h"

namespace coin {
namespace blk {

class BlockBuilder {
 public:
  static Block BuildGenesisBlock();
};

}  // namespace blk
}  // namespace coin

#endif
