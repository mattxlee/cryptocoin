#include "transaction.h"

namespace coin {

TransactionBase::TransactionBase() { time_ = time(nullptr); }

namespace tx {

data::Buffer MakeTxSignature(const ecdsa::Key &key, const data::Buffer &tx_hash,
                             int out_index) {
  // Hash with sha256 algorithm.
  Hash256Builder hash_builder;
  hash_builder << tx_hash << data::MakeValue(out_index);
  auto hash = hash_builder.FinalValue();

  // Make signature to hash value.
  return key.Sign(hash.get_data());
}

}  // namespace tx

void Transaction::set_pub_key(const data::Buffer &pub_key) {
  pub_key_ = pub_key;
}

void Transaction::add_tx_in(const TxIn &in) { vec_txin.push_back(in); }

void Transaction::add_tx_out(const TxOut &out) { vec_txout.push_back(out); }

}  // namespace coin
