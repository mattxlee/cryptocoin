#ifndef __TRANSACTION_H__
#define __TRANSACTION_H__

#include <string>
#include <vector>

#include "hash_utils.h"
#include "key.h"

namespace coin {

/// Basic transaction.
class TransactionBase {
 public:
  /// New transaction type can inherite from.
  virtual ~TransactionBase() {}

  /// Transaction type value, starts from 0.
  virtual int get_type() const = 0;

  /// Calculate hash value.
  virtual DataValue CalcHash() const = 0;
};

/// Transaction incoming tx.
struct TxIn {
  DataValue tx_hash;    // From transaction hash value.
  int out_index;        // txout index.
  DataValue signature;  // Signature of hash(tx_hash + out_index).

  DataValue CalcHashData() const {
    Hash256Builder hash_builder;
    hash_builder << tx_hash << ToDataValue(out_index) << signature;
    return hash_builder.FinalValue();
  }
};

/// Transaction outcoming tx.
struct TxOut {
  std::string address;  // To address.
  uint64_t value;       // Value.

  DataValue CalcHash() const {
    Hash256Builder hash_builder;
    hash_builder << ToDataValue(address) << ToDataValue(value);
    return hash_builder.FinalValue();
  }
};

namespace tx {

/**
 * @brief Make a signature for tx.
 *
 * @param key Private key to make signature.
 * @param tx_hash Transaction hash value.
 * @param out_index Out index.
 *
 * @return Signature data.
 */
DataValue MakeTxSignature(const ecdsa::Key &key, const DataValue &tx_hash,
                          int out_index);

}  // namespace tx

/// Spend transaction.
class Transaction : public TransactionBase {
 public:
  /// The type of current transaction.
  virtual int get_type() const override { return 0; }

  /// Set public key, verification of TxIn.
  void set_pub_key(const DataValue &pub_key);

  /// Add TxIn record.
  void add_tx_in(TxIn in);

  /// Add TxOut record.
  void add_tx_out(TxOut out);

  /// Serialize to stream.
  template <typename Stream>
  size_t Serialize(Stream &s) const {
  }

  /// Unserialize from stream.
  template <typename Stream>
  size_t Unserialize(const Stream &s) {}

  /// Calculate hash value.
  virtual DataValue CalcHash() const override;

 private:
  DataValue pub_key_;
  std::vector<TxIn> vec_txin;
  std::vector<TxOut> vec_txout;
};

}  // namespace coin

#endif
