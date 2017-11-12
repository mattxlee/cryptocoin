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
  void Serialize(Stream &s) {
    data::MakeValue(version_).WriteToStream(s);
    data::MakeValue(timestamp_).WriteToStream(s);
    data::MakeValue(height_).WriteToStream(s);
    data::MakeValue(block_hash_).WriteToStream(s);
    data::MakeValue(prev_hash_).WriteToStream(s);
    data::MakeValue(merkle_root_hash_).WriteToStream(s);
    data::MakeValue(nonce_).WriteToStream(s);
    data::MakeValue(difficult_).WriteToStream(s);
    data::MakeValue(trans_).WriteToStream(s);
  }

  template <typename Stream>
  void Unserialize(Stream &s) {
    data::MakeValue(version_).ReadFromStream(s);
    data::MakeValue(timestamp_).ReadFromStream(s);
    data::MakeValue(height_).ReadFromStream(s);
    data::MakeValue(block_hash_).ReadFromStream(s);
    data::MakeValue(prev_hash_).ReadFromStream(s);
    data::MakeValue(merkle_root_hash_).ReadFromStream(s);
    data::MakeValue(nonce_).ReadFromStream(s);
    data::MakeValue(difficult_).ReadFromStream(s);
    data::MakeValue(trans_).ReadFromStream(s);
  }

  void set_block_hash(const bn::HashNum &num);
  const bn::HashNum &get_block_hash() const;

  void set_timestamp(time_t timestamp);
  time_t get_timestamp() const;

  void set_height(uint32_t height);
  uint32_t get_height() const;

  void set_prev_hash(const bn::HashNum &num);
  const bn::HashNum &get_prev_hash() const;

  void set_merkle_root_hash(const bn::HashNum &hash);
  const bn::HashNum &get_merkle_root_hash() const;

  void set_nonce(uint32_t nonce);
  uint32_t get_nonce() const;

  void set_difficult(const bn::HashNum &difficult);
  const bn::HashNum &get_difficult() const;

  std::vector<Transaction> &get_trans();
  const std::vector<Transaction> &get_trans() const;

 private:
  // Block base info.
  int version_ = 1;
  time_t timestamp_ = 0;
  uint32_t height_ = -1;
  bn::HashNum block_hash_;
  // Below values contain in the calculation of block hash.
  bn::HashNum prev_hash_;
  bn::HashNum merkle_root_hash_;
  uint32_t nonce_ = 0;
  bn::HashNum difficult_;
  std::vector<Transaction> trans_;
};

}  // namespace blk
}  // namespace coin

#endif
