#include "transaction.h"

namespace coin {

namespace tx {

DataValue MakeTxSignature(const ecdsa::Key &key, const DataValue &tx_hash,
                          int out_index) {
  // Prepare data to hash.
  int out_index_n = htonl(out_index);
  DataValue hash_data(tx_hash.size() + sizeof(out_index_n));
  memcpy(hash_data.data(), tx_hash.data(), tx_hash.size());
  memcpy(hash_data.data() + tx_hash.size(), &out_index_n, sizeof(out_index_n));

  // Hash with sha256 algorithm.
  Hash256Builder hash_builder;
  hash_builder << hash_data;
  DataValue hash = hash_builder.FinalValue();

  // Make signature to hash value.
  DataValue signature;
  bool succ;
  std::tie(signature, succ) = key.Sign(hash);
  if (!succ) {
    // TODO Something wrong!
  }

  // Return signature data.
  return signature;
}

}  // namespace tx

void Transaction::set_pub_key(const DataValue &pub_key) {
  pub_key_ = pub_key;
}

void Transaction::add_tx_in(TxIn in) {
  vec_txin.emplace_back(std::move(in));
}

void Transaction::add_tx_out(TxOut out) {
  vec_txout.emplace_back(std::move(out));
}

DataValue Transaction::CalcHash() const {}

}  // namespace coin
