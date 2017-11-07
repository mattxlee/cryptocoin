#ifndef __TRANSACTION_H__
#define __TRANSACTION_H__

#include <string>
#include <vector>

#include "hash_utils.h"
#include "key.h"
#include "merkle_tree.h"

namespace coin {

/// Basic transaction.
class TransactionBase {
 public:
  /// Create new Transaction.
  TransactionBase();

  /// New transaction type can inherite from.
  virtual ~TransactionBase() {}

  /// Get timestamp.
  time_t get_time() const { return time_; }

  /// Transaction type value, starts from 0.
  virtual int get_type() const = 0;

  /// Calculate hash value.
  virtual DataValue CalcHash() const = 0;

 private:
  time_t time_ = 0;
};

/// Transaction incoming tx.
struct TxIn {
  DataValue tx_hash;    // From transaction hash value.
  int out_index;        // txout index.
  DataValue signature;  // Signature of hash(tx_hash + out_index).

  DataValue CalcHash() const {
    Hash256Builder hash_builder;
    hash_builder << tx_hash << ToDataValue(out_index) << signature;
    return hash_builder.FinalValue();
  }

  template <typename Stream>
  void Serialize(Stream &s) {
    s << tx_hash;
    s << ToDataValue(out_index);
    s << signature;
  }
};

/// Transaction outcoming tx.
struct TxOut {
  std::string address;  // To address.
  uint64_t amount;      // Transfer amount.

  DataValue CalcHash() const {
    Hash256Builder hash_builder;
    hash_builder << ToDataValue(address) << ToDataValue(amount);
    return hash_builder.FinalValue();
  }

  template <typename Stream>
  void Serialize(Stream &s) {
    s << ToDataValue(address);
    s << ToDataValue(amount);
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
  void add_tx_in(const TxIn &in);

  /// Add TxOut record.
  void add_tx_out(const TxOut &out);

  /// Serialize to stream.
  template <typename Stream>
  size_t Serialize(Stream &s) const {
    // Header values.
    s << ToDataValue(get_type());                    // type
    s << ToDataValue(static_cast<int>(get_time()));  // timestamp
    s << pub_key_;                                   // public key

    // Tx in/out merkle tree hash value.
    mt::NodePtr txin_root = mt::MakeMerkleTree(vec_txin);    // TxIn
    mt::NodePtr txout_root = mt::MakeMerkleTree(vec_txout);  // TxOut
    Hash256Builder hash_builder;
    hash_builder << txin_root->get_hash() << txout_root->get_hash();
    s << hash_builder.FinalValue();

    // TxIn list.
    s << ToDataValue(static_cast<int>(vec_txin.size()));
    for (const TxIn &in : vec_txin) {
      in.Serialize(s);
    }

    // TxOut list.
    s << ToDataValue(static_cast<int>(vec_txout.size()));
    for (const TxOut &out : vec_txout) {
      out.Serialize(s);
    }
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
