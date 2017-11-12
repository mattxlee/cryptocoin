#include "block.h"

namespace coin {
namespace blk {

void Block::set_block_hash(const bn::HashNum &num) { block_hash_ = num; }

const bn::HashNum &Block::get_block_hash() const { return block_hash_; }

void Block::set_timestamp(time_t timestamp) { timestamp_ = timestamp; }

time_t Block::get_timestamp() const { return timestamp_; }

void Block::set_height(uint32_t height) { height_ = height; }

uint32_t Block::get_height() const { return height_; }

void Block::set_prev_hash(const bn::HashNum &num) { prev_hash_ = num; }

const bn::HashNum &Block::get_prev_hash() const { return prev_hash_; }

void Block::set_merkle_root_hash(const bn::HashNum &hash) {
  merkle_root_hash_ = hash;
}

const bn::HashNum &Block::get_merkle_root_hash() const {
  return merkle_root_hash_;
}

void Block::set_nonce(uint32_t nonce) { nonce_ = nonce; }

uint32_t Block::get_nonce() const { return nonce_; }

void Block::set_difficult(const bn::HashNum &difficult) {
  difficult_ = difficult;
}

const bn::HashNum &Block::get_difficult() const { return difficult_; }

std::vector<Transaction> &Block::get_trans() { return trans_; }

const std::vector<Transaction> &Block::get_trans() const { return trans_; }

}  // namespace blk

}  // namespace coin
