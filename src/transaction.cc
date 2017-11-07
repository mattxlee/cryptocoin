#include "transaction.h"

namespace coin {

TransactionBase::TransactionBase() { time_ = time(nullptr); }

namespace tx {

DataValue MakeTxSignature(const ecdsa::Key &key, const DataValue &tx_hash,
                          int out_index) {
  // Hash with sha256 algorithm.
  Hash256Builder hash_builder;
  hash_builder << tx_hash << ToDataValue(out_index);
  DataValue hash = hash_builder.FinalValue();

  // Make signature to hash value.
  return key.Sign(hash);
}

}  // namespace tx

void Transaction::set_pub_key(const DataValue &pub_key) { pub_key_ = pub_key; }

void Transaction::add_tx_in(const TxIn &in) { vec_txin.push_back(in); }

void Transaction::add_tx_out(const TxOut &out) { vec_txout.push_back(out); }

DataValue Transaction::CalcHash() const {}

}  // namespace coin
